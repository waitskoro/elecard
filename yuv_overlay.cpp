#include "yuv_overlay.hpp"
#include "bmp_reader.hpp"
#include "rgb_to_yuv.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstring>

void overlay_image_on_yuv(YuvFrame& frame, const RgbImage& overlay, 
                          uint32_t pos_x, uint32_t pos_y) 
{
    if (pos_x >= frame.width || pos_y >= frame.height) return;

    uint32_t max_width = std::min(overlay.width, frame.width - pos_x);
    uint32_t max_height = std::min(overlay.height, frame.height - pos_y);

    std::vector<uint8_t> overlay_yuv;
    overlay_yuv.resize(max_width * max_height + 2 * (max_width / 2) * (max_height / 2));
    rgb_to_yuv420(overlay.data.data(), max_width, max_height, overlay_yuv.data());

    uint8_t* frame_y = frame.data.data();
    uint8_t* frame_u = frame.data.data() + frame.width * frame.height;
    uint8_t* frame_v = frame_u + (frame.width / 2) * (frame.height / 2);

    uint8_t* overlay_y = overlay_yuv.data();
    uint8_t* overlay_u = overlay_yuv.data() + max_width * max_height;
    uint8_t* overlay_v = overlay_u + (max_width / 2) * (max_height / 2);

    for (uint32_t y = 0; y < max_height; ++y) {
        memcpy(frame_y + (pos_y + y) * frame.width + pos_x,
               overlay_y + y * max_width,
               max_width);
    }

    for (uint32_t y = 0; y < max_height; y += 2) {
        for (uint32_t x = 0; x < max_width; x += 2) {
            uint32_t uv_x = x / 2;
            uint32_t uv_y = y / 2;
            uint32_t frame_uv_idx = ( (pos_y + y) / 2 ) * (frame.width / 2) + (pos_x + x) / 2;
            uint32_t overlay_uv_idx = uv_y * (max_width / 2) + uv_x;

            frame_u[frame_uv_idx] = overlay_u[overlay_uv_idx];
            frame_v[frame_uv_idx] = overlay_v[overlay_uv_idx];
        }
    }
}

void process_video(const std::string& input_yuv, const std::string& bmp_file,
                   const std::string& output_yuv, uint32_t width, uint32_t height) 
{
    RgbImage overlay = read_bmp(bmp_file);

    if (overlay.width > width || overlay.height > height) {
        throw std::runtime_error("Ошибка: Изображение для наложения больше кадра видео");
    }

    std::ifstream input(input_yuv, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Не удалось открыть входной YUV-файл");
    }

    std::ofstream output(output_yuv, std::ios::binary);
    if (!output.is_open()) {
        throw std::runtime_error("Не удалось создать выходной YUV-файл");
    }

    uint32_t frame_size = width * height * 3 / 2;
    std::vector<uint8_t> frame_buffer(frame_size);
    
    input.seekg(0, std::ios::end);
    size_t file_size = input.tellg();
    input.seekg(0, std::ios::beg);
    
    while (input.read(reinterpret_cast<char*>(frame_buffer.data()), frame_size)) {
        YuvFrame frame { 
            width, 
            height, 
            frame_buffer 
        };

        overlay_image_on_yuv(frame, overlay, 0, 0);

        output.write(reinterpret_cast<const char*>(frame.data.data()), frame_size);
    }
}
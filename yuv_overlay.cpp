#include "yuv_overlay.hpp"

#include "bmp_reader.hpp"
#include "rgb_to_yuv.hpp"

#include <fstream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

Frame prepare_overlay(const Frame& overlay) 
{
    Frame prepared;
    prepared.width = overlay.width;
    prepared.height = overlay.height;
    
    uint32_t y_plane_size = overlay.width * overlay.height;
    uint32_t uv_plane_size = (overlay.width / 2) * (overlay.height / 2);
    prepared.data.resize(y_plane_size + 2 * uv_plane_size);
    
    rgb_to_yuv420(overlay.data.data(), overlay.width, overlay.height, prepared.data.data());
    
    return prepared;
}

void overlay_image_on_yuv(Frame& frame, const Frame& overlay, 
                          uint32_t pos_x, uint32_t pos_y) 
{
    if (pos_x >= frame.width || pos_y >= frame.height) return;

    uint32_t max_width = std::min(overlay.width, frame.width - pos_x);
    uint32_t max_height = std::min(overlay.height, frame.height - pos_y);

    const uint8_t* overlay_y = overlay.data.data();
    const uint8_t* overlay_u = overlay.data.data() + overlay.width * overlay.height;
    const uint8_t* overlay_v = overlay_u + (overlay.width / 2) * (overlay.height / 2);

    uint8_t* frame_y = frame.data.data();
    uint8_t* frame_u = frame.data.data() + frame.width * frame.height;
    uint8_t* frame_v = frame_u + (frame.width / 2) * (frame.height / 2);

    for (uint32_t y = 0; y < max_height; ++y) {
        memcpy(frame_y + (pos_y + y) * frame.width + pos_x,
               overlay_y + y * overlay.width,
               max_width);
    }

    for (uint32_t y = 0; y < max_height; y += 2) {
        for (uint32_t x = 0; x < max_width; x += 2) {
            uint32_t uv_x = x / 2;
            uint32_t uv_y = y / 2;

            uint32_t frame_uv_idx = ((pos_y + y) / 2) * (frame.width / 2) + (pos_x + x) / 2;

            uint32_t overlay_uv_idx = uv_y * (overlay.width / 2) + uv_x;

            frame_u[frame_uv_idx] = overlay_u[overlay_uv_idx];
            frame_v[frame_uv_idx] = overlay_v[overlay_uv_idx];
        }
    }
}

void process_video(const std::string& input_yuv, const std::string& bmp_file,
                   const std::string& output_yuv, uint32_t width, uint32_t height) 
{
    Frame overlay = read_bmp(bmp_file);

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

    Frame overlay = prepare_overlay(overlay);

    uint32_t frame_size = width * height * 3 / 2;
    std::vector<uint8_t> frame_buffer(frame_size);

    input.seekg(0, std::ios::end);
    size_t file_size = input.tellg();
    input.seekg(0, std::ios::beg);

    while (input.read(reinterpret_cast<char*>(frame_buffer.data()), frame_size)) {
        Frame frame { 
            width, 
            height, 
            frame_buffer 
        };

        overlay_image_on_yuv(frame, overlay, 0, 0);

        output.write(reinterpret_cast<const char*>(frame.data.data()), frame_size);
    }
}
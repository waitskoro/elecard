#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "bmp_reader.hpp"

struct YuvFrame {
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> data;
};

void overlay_image_on_yuv(YuvFrame& frame, const RgbImage& overlay, 
                          uint32_t pos_x, uint32_t pos_y);

void process_video(const std::string& input_yuv, const std::string& bmp_file,
                   const std::string& output_yuv, uint32_t width, uint32_t height);
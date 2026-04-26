#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "bmp_reader.hpp"

Frame prepare_overlay(const Frame& overlay);

void overlay_image_on_yuv(Frame& frame, const Frame& overlay, 
                          uint32_t pos_x, uint32_t pos_y);

void process_video(const std::string& input_yuv, const std::string& bmp_file,
                   const std::string& output_yuv, uint32_t width, uint32_t height);
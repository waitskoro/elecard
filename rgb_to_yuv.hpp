#pragma once

#include <cstdint>
#include <vector>

void rgb_to_yuv(uint8_t r, uint8_t g, uint8_t b,
                uint8_t& y, uint8_t& u, uint8_t& v);

void rgb_to_yuv420(const uint8_t* rgb_data, uint32_t width, 
                   uint32_t height, uint8_t* yuv_data); 
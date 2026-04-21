#include "rgb_to_yuv.hpp"

#include <cstdint>
#include <algorithm>

void rgb_to_yuv(uint8_t r, uint8_t g, uint8_t b,
    uint8_t& y, uint8_t& u, uint8_t& v)
{
    y = (uint8_t)(( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16;
    u = (uint8_t)((-38 * r -  74 * g + 112 * b + 128) >> 8) + 128;
    v = (uint8_t)((112 * r -  94 * g -  18 * b + 128) >> 8) + 128;
}

void rgb_to_yuv420(const uint8_t* rgb_data, uint32_t width, uint32_t height, uint8_t* yuv_data) 
{
    uint32_t y_plane_size = width * height;
    uint32_t uv_plane_size = (width / 2) * (height / 2);

    uint8_t* y_plane = yuv_data;
    uint8_t* u_plane = yuv_data + y_plane_size;
    uint8_t* v_plane = u_plane + uv_plane_size;

    for (uint32_t y = 0; y < height; y += 2) {
        for (uint32_t x = 0; x < width; x += 2) {
            uint32_t sum_u = 0, sum_v = 0;

            for (uint32_t dy = 0; dy < 2 && (y + dy) < height; ++dy) {
                for (uint32_t dx = 0; dx < 2 && (x + dx) < width; ++dx) {
                    const uint8_t* rgb = rgb_data + ((y + dy) * width + (x + dx)) * 3;
                    uint8_t r = rgb[0];
                    uint8_t g = rgb[1];
                    uint8_t b = rgb[2];

                    uint8_t y_val, u_val, v_val;
                    rgb_to_yuv(r, g, b, y_val, u_val, v_val);

                    y_plane[(y + dy) * width + (x + dx)] = y_val;

                    sum_u += u_val;
                    sum_v += v_val;
                }
            }

            uint32_t block_index = (y / 2) * (width / 2) + (x / 2);
            u_plane[block_index] = sum_u / 4;
            v_plane[block_index] = sum_v / 4;
        }
    }
}
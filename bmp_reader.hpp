#pragma once

#include <vector>
#include <string>
#include <cstdint>

struct RgbImage {
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> data;
};

#pragma pack(push, 1)

struct BmpHeader {
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
};

struct BmpInfoHeader {
    uint32_t header_size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t  x_pixels_per_meter;
    int32_t  y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
};

#pragma pack(pop)

RgbImage read_bmp(const std::string& filename);

bool validate_bmp_header(const uint8_t* header);

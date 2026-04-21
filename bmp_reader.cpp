#include "bmp_reader.hpp"

#include <fstream>
#include <cstring>
#include <stdexcept>

RgbImage read_bmp(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Невозможно открыть BMP файл: " + filename);
    }

    BmpHeader header;
    BmpInfoHeader info_header;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    if (header.signature != 0x4D42) {
        throw std::runtime_error("Не является BMP файлом");
    }

    if (info_header.bits_per_pixel != 24) {
        throw std::runtime_error("Поддерживается только 24-битный RGB BMP");
    }

    if (info_header.compression != 0) {
        throw std::runtime_error("Сжатый BMP не поддерживается");
    }

    bool top_down = (info_header.height < 0);
    uint32_t abs_height = top_down ? -info_header.height : info_header.height;
    uint32_t width = info_header.width;

    file.seekg(header.data_offset, std::ios::beg);

    uint32_t row_stride = (width * 3 + 3) & ~3;
    std::vector<uint8_t> buffer(abs_height * row_stride);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    RgbImage result;
    result.width = width;
    result.height = abs_height;
    result.data.resize(width * abs_height * 3);

    for (uint32_t y = 0; y < abs_height; ++y) {
        uint32_t src_y = top_down ? y : (abs_height - 1 - y);
        const uint8_t* src_row = buffer.data() + src_y * row_stride;
        uint8_t* dst_row = result.data.data() + y * width * 3;

        for (uint32_t x = 0; x < width; ++x) {
            dst_row[x * 3 + 0] = src_row[x * 3 + 2]; // R
            dst_row[x * 3 + 1] = src_row[x * 3 + 1]; // G
            dst_row[x * 3 + 2] = src_row[x * 3 + 0]; // B
        }
    }

    return result;
}
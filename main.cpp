#include "yuv_overlay.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

void print_usage(const char* program_name) 
{
    std::cout << "Использование: " << program_name << " <input.yuv> <overlay.bmp> <output.yuv> <ширина> <высота>" << std::endl;
    std::cout << std::endl;
    std::cout << "Аргументы:" << std::endl;
    std::cout << "  input.yuv   - Входной YUV420 видеофайл" << std::endl;
    std::cout << "  overlay.bmp - BMP изображение для наложения (24-битный RGB)" << std::endl;
    std::cout << "  output.yuv  - Выходной YUV420 видеофайл" << std::endl;
    std::cout << "  ширина      - Ширина кадра видео (например, 352, 1920, 1280)" << std::endl;
    std::cout << "  высота      - Высота кадра видео (например, 288, 1080, 720)" << std::endl;
    std::cout << std::endl;
    std::cout << "Пример:" << std::endl;
    std::cout << "  " << program_name << " video.yuv image.bmp output.yuv 352 288" << std::endl;
    std::cout << std::endl;
    std::cout << "Требования:" << std::endl;
    std::cout << "  - YUV файл должен быть в planar формате YUV420" << std::endl;
    std::cout << "  - BMP должен быть 24-битным RGB, без сжатия, без палитры" << std::endl;
    std::cout << "  - Размер наложения должен быть <= размера кадра видео" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc != 6) {
        print_usage(argv[0]);
        return 1;
    }

    uint32_t width = static_cast<uint32_t>(std::atoi(argv[4]));
    uint32_t height = static_cast<uint32_t>(std::atoi(argv[5]));
    
    if (width == 0 || height == 0) {
        throw std::runtime_error("Неверный размер видео: ширина и высота должны быть положительными числами");
    }

    try {
        std::cout << "=== Используемые данные ===" << std::endl;
        std::cout << "Входной YUV:   " << argv[1] << std::endl;
        std::cout << "BMP для наложения: " << argv[2] << std::endl;
        std::cout << "Выходной YUV:  " << argv[3] << std::endl;
        std::cout << "Размер видео:  " << width << "x" << height << std::endl;
        std::cout << "===========================" << std::endl;
        std::cout << std::endl;
        
        process_video(argv[1], argv[2], argv[3], width, height);

        std::cout << "Видео успешно обработано!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
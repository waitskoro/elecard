cmake_minimum_required(VERSION 3.5)

project(elecard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(elecard main.cpp
    bmp_reader.cpp
    bmp_reader.hpp
    rgb_to_yuv.hpp
    rgb_to_yuv.cpp
    yuv_overlay.hpp
    yuv_overlay.cpp
)

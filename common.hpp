#include <vector>
#include <cstdint>

struct Frame {
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> data;
};
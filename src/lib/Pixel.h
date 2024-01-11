#ifndef WSGL_PIXEL
#define WSGL_PIXEL
#include <cstdint>

struct Pixel
{
    union
    {
        uint32_t n = 0xFF000000;
        struct
        {
            uint8_t r;	uint8_t g;	uint8_t b;	uint8_t a;
        };
    };

    Pixel();
    Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
    Pixel(uint32_t p);
    enum Mode { NORMAL, MASK, ALPHA, CUSTOM };
};

inline Pixel::Pixel()
{
    r = 0; g = 0; b = 0; a = 255;
}

inline Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    r = red; g = green; b = blue; a = alpha;
}

inline Pixel::Pixel(uint32_t p)
{
    n = p;
}

#endif
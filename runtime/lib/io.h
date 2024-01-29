#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

#ifndef DOG_ENGINE_IO
#define DOG_ENGINE_IO

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../lib/wfsgl32.h"
#include "../add_libs/include/png.h"

struct image {
    int width, height;
    std::vector<Pixel> pixels;
};

namespace IO {
    inline GLuint generateTexture(image im) {
        GLuint gpuTexture;

        glCreateTextures(GL_TEXTURE_2D, 1, &gpuTexture);

        glTextureParameteri(gpuTexture, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTextureParameteri(gpuTexture, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTextureParameteri(gpuTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(gpuTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(gpuTexture, 1, GL_RGBA8, im.width, im.height);
        glTextureSubImage2D(gpuTexture, 0, 0, 0, im.width, im.height, GL_RGBA, GL_UNSIGNED_BYTE, im.pixels.data());

        return gpuTexture;
    }

    inline image loadPNG(const char* filename) {
        image rgbData;

        FILE* file;

        fopen_s(&file, filename, "rb");
        if (!file) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return rgbData;
        }

        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png) {
            std::cerr << "Unable to create read struct" << std::endl;
            fclose(file);
            return rgbData;
        }

        png_infop info = png_create_info_struct(png);
        if (!info) {
            std::cerr << "Unable to create info struct" << std::endl;
            png_destroy_read_struct(&png, nullptr, nullptr);
            fclose(file);
            return rgbData;
        }

        if (setjmp(png_jmpbuf(png))) {
            std::cerr << "Error during png read" << std::endl;
            png_destroy_read_struct(&png, &info, nullptr);
            fclose(file);
            return rgbData;
        }

        png_init_io(png, file);
        png_read_info(png, info);

        png_byte colorType = png_get_color_type(png, info);
        png_byte bitDepth = png_get_bit_depth(png, info);

        if (bitDepth == 16) {
            png_set_strip_16(png);
        }

        if (colorType == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png);
        }

        if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
            png_set_expand_gray_1_2_4_to_8(png);
        }

        if (png_get_valid(png, info, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png);
        }

        if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

        if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png);
        }

        png_read_update_info(png, info);

        int width = png_get_image_width(png, info);
        int height = png_get_image_height(png, info);

        png_bytep* rowPointers = new png_bytep[height];
        for (int y = 0; y < height; y++) {
            rowPointers[y] = new png_byte[png_get_rowbytes(png, info)];
        }

        png_read_image(png, rowPointers);

        fclose(file);

        rgbData.pixels.resize(width * height);
        rgbData.width = width;
        rgbData.height = height;
        int channels = png_get_channels(png, info);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                png_bytep pixel = &(rowPointers[y][x * channels]);
                rgbData.pixels[y * width + x].r = pixel[0];
                rgbData.pixels[y * width + x].g = pixel[1];
                rgbData.pixels[y * width + x].b = pixel[2];
            }
        }

        for (int y = 0; y < height; y++) {
            delete[] rowPointers[y];
        }
        delete[] rowPointers;

        png_destroy_read_struct(&png, &info, nullptr);

        return rgbData;
    }

}

#endif
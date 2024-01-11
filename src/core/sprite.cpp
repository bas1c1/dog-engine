#include "definitions.h"
#include "../lib/io.h"

sprite::sprite() {
	width = 0;
	height = 0;
	pixels = std::vector<Pixel>();
}

sprite::sprite(int width, int height) {
	this->width = width;
	this->height = height;
	this->pixels = std::vector<Pixel>(width * height);
	for (int i = 0; i < width * height; i++)
		pixels[i] = Pixel(255, 255, 255);

	GenerateGPUTexture();
}

sprite::sprite(int width, int height, std::vector<Pixel> pixels) {
	this->width = width;
	this->height = height;
	this->pixels = pixels;

	GenerateGPUTexture();
}

sprite::sprite(std::string filename) {
	image im = IO::loadPNG(filename.c_str());
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;

	GenerateGPUTexture();
}

void sprite::fromFile(std::string filename) {
	image im = IO::loadPNG(filename.c_str());
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;
	GenerateGPUTexture();
}

bool sprite::FillArea(vec2d f, vec2d s, Pixel color) {
	if (
		(f.x < 0 || f.x > width) ||
		(s.x < 0 || s.x > width) ||
		(f.y < 0 || f.y > height) ||
		(f.y < 0 || f.y > height)
		)
		return false;

	for (int x = f.x; x < s.x; x++) {
		for (int y = f.y; y < s.y; y++) {
			pixels[x + y * width] = color;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

	return true;
}

void sprite::GenerateGPUTexture() {
	glGenTextures(1, &gpuTexture);
	glBindTexture(GL_TEXTURE_2D, gpuTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}
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

	this->gpuTexture = IO::generateTexture({ width, height, pixels });
}

sprite::sprite(int width, int height, std::vector<Pixel> pixels) {
	this->width = width;
	this->height = height;
	this->pixels = pixels;

	this->gpuTexture = IO::generateTexture({width, height, pixels});
}

sprite::sprite(std::string filename) {
	image im = IO::loadPNG(filename.c_str());
	this->gpuTexture = IO::generateTexture(im);
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;
}

void sprite::fromFile(std::string filename) {
	image im = IO::loadPNG(filename.c_str());
	this->gpuTexture = IO::generateTexture(im);
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;
}
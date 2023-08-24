#include "definitions.h"
#include "../lib/io.h"

sprite::sprite() {
	width = 0;
	height = 0;
	pixels = std::vector<Pixel>(0);
}

sprite::sprite(int width, int height, camera& cam2d) {
	this->width = width;
	this->height = height;
	this->pixels = std::vector<Pixel>(width * height);
	for (int i = 0; i < width * height; i++)
		pixels[i] = Pixel(255, 255, 255);
	this->cam2d = cam2d;

	GenerateGPUTexture();
}

sprite::sprite(int width, int height, std::vector<Pixel> pixels, camera& cam2d) {
	this->width = width;
	this->height = height;
	this->pixels = pixels;
	this->cam2d = cam2d;

	GenerateGPUTexture();
}

sprite::sprite(const char* filename, int width, int height, camera& cam2d) {
	this->pixels = IO::loadPNG(filename);
	this->width = width;
	this->height = height;
	this->cam2d = cam2d;

	GenerateGPUTexture();
}

void sprite::bindTransform(transform* tr) {
	this->tr = tr;
}

void sprite::update() {
	this->cam2d = cam2d;

	this->localPos = tr->getPos();
	this->localScale = tr->getScale();
	this->localRotationAngle = tr->getRotationAngle();

	glBindTexture(GL_TEXTURE_2D, gpuTexture);
	writeSprite(tr->square);
}

void sprite::GenerateGPUTexture() {
	glGenTextures(1, &gpuTexture);
	glBindTexture(GL_TEXTURE_2D, gpuTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
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
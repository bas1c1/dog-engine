#include "definitions.h"
#include "../lib/io.h"

sprite::sprite() {
	width = 0;
	height = 0;
	pixels = std::vector<Pixel>(0);
	localScale = vec2d(1, 1);
}

sprite::sprite(int width, int height, camera& cam2d) {
	this->width = width;
	this->height = height;
	this->pixels = std::vector<Pixel>(width * height);
	for (int i = 0; i < width * height; i++)
		pixels[i] = Pixel(255, 255, 255);
	this->cam2d = cam2d;
	this->localScale = vec2d(1, 1);

	GenerateGPUTexture();
}

sprite::sprite(int width, int height, std::vector<Pixel> pixels, camera& cam2d) {
	this->width = width;
	this->height = height;
	this->pixels = pixels;
	this->cam2d = cam2d;
	this->localScale = vec2d(1, 1);

	GenerateGPUTexture();
}

sprite::sprite(std::string filename, camera& cam2d) {
	image im = IO::loadPNG(filename.c_str());
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;
	this->cam2d = cam2d;
	this->localScale = vec2d(1, 1);

	GenerateGPUTexture();
}

void sprite::bindTransform(transform* tr) {
	this->tr = tr;
	this->localPos = tr->getPos();
	this->localRotationAngle = tr->getRotationAngle();
}

void sprite::scaleObj(vec2d scale) {
	for (int i = 0; i < 4; i++) {
		tr->square[i].x = tr->square[i].x * scale.x;
		tr->square[i].y = tr->square[i].y * scale.y;
	}
}

void sprite::update() {
	glBindTexture(GL_TEXTURE_2D, gpuTexture);

	scaleObj(localScale);

	writeSprite(tr->drawable_square);
}

std::string sprite::toString()
{
	return std::string("sprite");
}

vec2d sprite::getScale() {
	return localScale;
}

void sprite::setScale(vec2d scale) {
	this->localScale = scale;
}

void sprite::fromFile(std::string filename) {
	image im = IO::loadPNG(filename.c_str());
	this->pixels = im.pixels;
	this->filename = filename;
	this->width = im.width;
	this->height = im.height;
	GenerateGPUTexture();
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
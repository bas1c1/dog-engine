#include "definitions.h"
#include "../lib/io.h"

spriteRender::spriteRender() {}

void spriteRender::bindTransform(transform* tr) {
	this->tr = tr;
}

sprite* spriteRender::getSprite() {
	return this->sp;
}

void spriteRender::bindSprite(sprite* sp) {
	this->sp = sp;
}

void spriteRender::update() {
	glBindTexture(GL_TEXTURE_2D, sp->gpuTexture);

	writeSprite(tr->drawable_square);
}

std::string spriteRender::toString()
{
	return std::string("spriteRender");
}
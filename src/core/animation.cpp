#include "definitions.h"

extern float deltaTime;

animation::animation() {}

animation::animation(int delay) {
	this->delay = delay;
}

void animation::bindTransform(transform* tr) {
	this->tr = tr;
}

void animation::addSprite(sprite* sp) {
	this->sps.push_back(sp);
}

int animation::getDelay() {
	return this->delay;
}

void animation::setDelay(int delay) {
	this->delay = delay;
}

std::vector<sprite*> animation::getAllFrames() {
	return this->sps;
}

int index = 0;
int64_t globalFrameCounter = 0;

void animation::update() {
	if (this->delay == 0) {
		this->delay = 1;
	}

	if ((globalFrameCounter % this->delay) == 0) {
		if (index + 1 >= this->sps.size())
			index = 0;
		else
			index += 1;
	}
	sprite* currentSprite = this->sps[index];
	
	glBindTexture(GL_TEXTURE_2D, currentSprite->gpuTexture);

	writeSprite(tr->drawable_square);

	globalFrameCounter += 1;
}

std::string animation::toString()
{
	return std::string("animation");
}
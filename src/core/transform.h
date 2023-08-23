#ifndef DOG_ENGINE_TRANSFORM
#define DOG_ENGINE_TRANSFORM

#include "../../ext/include/wfsgl32.h"
#include "component.h"

class transform : public base_component {
private:
	vec2d pos;
public:
	float localScale;

	transform() : pos(vec2d().zero()), localScale(0.0f) { }

	transform(vec2d pos, float localScale = 0.0f) {
		this->pos = pos;
		this->localScale = localScale;
	}

	void update() override {

	}

	void moveToPos(vec2d newPos) {
		this->pos = pos + newPos;
	}

	vec2d getPos() { return pos; }
};

#endif
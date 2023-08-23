#ifndef DOG_ENGINE_CAMERA2D
#define DOG_ENGINE_CAMERA2D

#include "../../ext/include/wfsgl32.h"
#include "object.h"

class camera : object {
private:
	vec2d uv;
	vec2d pos;

public:
	camera() {
		this->uv = vec2d().zero();
		this->add_component<transform>(transform(vec2d().zero()));
		this->pos = vec2d().zero();
	}

	camera(int width, int height, vec2d pos, WFSGL ctx) {
		this->uv = vec2d(width, height);
		this->add_component<transform>(transform(pos));
		this->pos = pos;
	}

	void update() override {
		this->pos = get_transfrom()->getPos();
	}

	object gameObject() {
		return object::gameObject();
	}

#define FASTPROJECTX(x, width, camx)  x - camx + width /  2
#define FASTPROJECTY(y, height, camy) y - camy + height / 2

	inline vec2d project(vec2d point) {
		return vec2d(FASTPROJECTX(point.x, uv.x, pos.x), FASTPROJECTX(point.y, uv.y, pos.y));
	}
};

static camera mainCamera = camera();

#endif
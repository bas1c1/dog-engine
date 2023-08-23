#ifndef DOG_ENGINE_CAMERA2D
#define DOG_ENGINE_CAMERA2D

#include "../../ext/include/wfsgl32.h"
#include "object.h"

class camera : object {
private:
	vec2d uv;
	vec2d pos;
	WFSGL ctx;

public:
	std::vector<Pixel> screenPixels;
	camera() {
		this->uv = vec2d().zero();
		this->add_component<transform>(transform(vec2d().zero()));
		this->pos = vec2d().zero();
	}

	camera(int width, int height, vec2d pos, WFSGL ctx) {
		this->uv = vec2d(width, height);
		this->add_component<transform>(transform(pos));
		this->pos = pos;
		this->ctx = ctx;
		this->screenPixels = std::vector<Pixel>(width * height);
	}

	void update() override {
		this->pos = get_transfrom()->getPos();
	}

	object gameObject() {
		return object::gameObject();
	}

#define FASTPROJECTX(x, width, camx)  x - camx + width /  2
#define FASTPROJECTY(y, height, camy) y - camy + height / 2

	inline vec2d project(float px, float py) {
		return vec2d(FASTPROJECTX(px, uv.x, pos .x), FASTPROJECTX(py, uv.y, pos.y));
	}
};

static camera mainCamera = camera();

#endif
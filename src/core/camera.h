#ifndef DOG_ENGINE_CAMERA2D
#define DOG_ENGINE_CAMERA2D

#include "../../ext/include/wfsgl32.h"
#include "object.h"
#include "transform.h"

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

	camera(int width, int height, vec2d pos) {
		this->uv = vec2d(width, height);
		this->add_component<transform>(transform(pos));
		this->pos = pos;
	}

	void update() override {
		this->uv = uv;
		this->pos = get_transfrom()->getPos();
	}

	object gameObject() {
		return object::gameObject();
	}

	inline vec2d project(vec2d point) {
		float projected_x = point.x - pos.x + uv.x / 2;
		float projected_y = point.y - pos.y + uv.y / 2;
		return vec2d(projected_x, projected_y);
	}
};

static camera mainCamera = camera();

#endif
#include "definitions.h"

camera::camera() {
	this->objType = ObjectType::CAMERA;
	this->uv = vec2d().zero();
	this->add_component<transform>();
	this->pos = vec2d().zero();
}

camera::camera(int width, int height, vec2d pos, WFSGL& ctx) {
	this->objType = ObjectType::CAMERA;
	this->uv = vec2d(width, height);
	this->add_component<transform>(width, height, pos, ctx, this->objType);
	this->pos = pos;
}

void camera::update() {
	this->pos = get_component<transform>()->getPos();
	glOrtho(-zoom, zoom, -zoom, zoom, -1.5, 1.5);
	glTranslatef(-pos.x / 100, -pos.y / 100, 0);
}

object camera::gameObject() {
	return object::gameObject();
}

vec2d camera::project(vec2d point) {
	return vec2d(FASTPROJECTX(point.x, uv.x), FASTPROJECTX(point.y, uv.y));
}

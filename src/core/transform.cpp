#include "definitions.h"

extern camera mainCamera;

transform::transform() : pos(vec2d().zero()), scale(vec2d(1, 1)), rotationAngle(0), height(0), width(0) { }

transform::transform(int width, int height, vec2d pos, WFSGL& ctx, ObjectType objType, vec2d localScale, float rotationAngle) {
	switch (objType) {
	case ObjectType::BASE:
		this->pos = pos - vec2d(width / 2, height / 2);
		break;
	case ObjectType::CAMERA:
		this->pos = pos;
		break;
	}
	this->scale = localScale;
	this->rotationAngle = rotationAngle;
	this->width = width;
	this->height = height;
	this->ctx = ctx;
}

void transform::update() {
	vec2d oldPos = pos;
	pos = mainCamera.project(pos);
	const vec2d offset(pos.x + width, pos.y + height);

	square = std::vector<vec2d>{
		vec2d(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(pos.y)),
		vec2d(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(offset.y)),
		vec2d(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(offset.y)),
		vec2d(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(pos.y))
	};

	scaleObj(scale);
	rotateObj(rotationAngle);
	this->pos = oldPos;
}

void transform::setPosition(vec2d newPos) {
	this->pos = pos;
}

void transform::moveToPos(vec2d newPos) {
	this->pos = pos + newPos;
}

void transform::scaleObj(vec2d scale) {
	this->scale = scale;
	for (int i = 0; i < 4; i++) {
		float translatedX = square[i].x;
		float translatedY = square[i].y;

		float scaledX = translatedX * scale.x;
		float scaledY = translatedY * scale.y;

		square[i].x = scaledX;
		square[i].y = scaledY;
	}
}

void transform::rotateObj(float rotationAngle) {
	this->rotationAngle = rotationAngle;
	float radians = this->rotationAngle * 3.1415926 / 180.0;

	for (int i = 0; i < 4; i++) {
		float translatedX = square[i].x;
		float translatedY = square[i].y;

		float rotatedX = translatedX * cos(radians) - translatedY * sin(radians);
		float rotatedY = translatedX * sin(radians) + translatedY * cos(radians);

		square[i].x = rotatedX;
		square[i].y = rotatedY;
	}
}

vec2d transform::getPos() { return pos; }
vec2d transform::getScale() { return scale; }
float transform::getRotationAngle() { return rotationAngle; };

void transform::setPos(vec2d pos) { this->pos = pos; }
void transform::setScale(vec2d scale) { this->scale = scale; }
void transform::setRotationAngle(float rotationAngle) { this->rotationAngle = rotationAngle; }
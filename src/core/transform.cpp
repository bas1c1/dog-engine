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

vec2d calculateCenter(const std::vector<vec2d>& square) {
	double centerX = 0.0;
	double centerY = 0.0;

	for (const vec2d& vertex : square) {
		centerX += vertex.x;
		centerY += vertex.y;
	}

	centerX /= square.size();
	centerY /= square.size();

	return vec2d(centerX, centerY);
}

/*square = std::vector<vec2d>{
		vec2d(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(pos.y)),
		vec2d(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(offset.y)),
		vec2d(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(offset.y)),
		vec2d(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(pos.y))
	};*/

void transform::update() {
	vec2d oldPos = pos;
	pos = mainCamera.project(pos);
	const vec2d offset(pos.x + width, pos.y + height);

	square = std::vector<vec2d>{
		vec2d((pos.x), (pos.y)),
		vec2d((pos.x), (offset.y)),
		vec2d((offset.x), (offset.y)),
		vec2d((offset.x), (pos.y))
	};
	
	//not good
	//rotateObj(rotationAngle);
	scaleObj(scale);

	drawable_square = std::vector<vec2d>{
		SCREEN_TO_NDC(square[0]),
		SCREEN_TO_NDC(square[1]),
		SCREEN_TO_NDC(square[2]),
		SCREEN_TO_NDC(square[3])
	};

	this->pos = oldPos;

	
}

std::string transform::toString()
{
	return std::string("transform");
}

void transform::setPosition(vec2d newPos) {
	this->pos = pos;
}

void transform::moveToPos(vec2d newPos) {
	this->pos = pos + newPos;
}

void transform::scaleObj(vec2d scale) {
	this->scale = scale;
	vec2d center = calculateCenter(square);
	for (int i = 0; i < 4; i++) {
		float translatedX = square[i].x - center.x;
		float translatedY = square[i].y - center.y;

		float scaledX = translatedX * scale.x;
		float scaledY = translatedY * scale.y;

		square[i].x = scaledX + center.x;
		square[i].y = scaledY + center.y;
	}
}

void transform::rotateObj(float rotationAngle) {
	/*this->rotationAngle = rotationAngle;
	vec2d center = calculateCenter(square);
	float angleRadians = this->rotationAngle * 3.1415926 / 180.0;

	for (vec2d& vertex : square) {
		double translatedX = vertex.x - center.x;
		double translatedY = vertex.y - center.y;

		double rotatedX = translatedX * cos(angleRadians) - translatedY * sin(angleRadians);
		double rotatedY = translatedX * sin(angleRadians) + translatedY * cos(angleRadians);

		vertex.x = rotatedX + center.x;
		vertex.y = rotatedY + center.y;
	}*/
}

vec2d transform::getPos() { return pos; }
vec2d transform::getScale() { return scale; }
float transform::getRotationAngle() { return rotationAngle; };
int transform::getWidth() { return width; }
int transform::getHeight() { return height; }

void transform::setPos(vec2d pos) { this->pos = pos; }
void transform::setScale(vec2d scale) { this->scale = scale; }
void transform::setRotationAngle(float rotationAngle) { this->rotationAngle = rotationAngle; }
void transform::setWidth(int width) { this->width = width; }
void transform::setHeight(int height) { this->height = height; }
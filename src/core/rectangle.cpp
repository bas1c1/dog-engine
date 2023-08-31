#include "definitions.h"

rectangle::rectangle() = default;

rectangle::rectangle(float x, float y, float width, float height) {
	this->pos.x = x;
	this->pos.y = y;
	this->offsetPos.x = width;
	this->offsetPos.y = height;
	this->square = std::vector<vec2d>{
		vec2d(PUBLIC_SCREEN_TO_NDC_X(pos.x), PUBLIC_SCREEN_TO_NDC_Y(pos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(pos.x), PUBLIC_SCREEN_TO_NDC_Y(offsetPos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(offsetPos.x), PUBLIC_SCREEN_TO_NDC_Y(offsetPos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(offsetPos.x), PUBLIC_SCREEN_TO_NDC_Y(pos.y))
	};
}

rectangle::rectangle(vec2d pos, vec2d offsetPos) {
	this->pos = pos;
	this->offsetPos = offsetPos;
	this->square = std::vector<vec2d>{
		vec2d(PUBLIC_SCREEN_TO_NDC_X(pos.x), PUBLIC_SCREEN_TO_NDC_Y(pos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(pos.x), PUBLIC_SCREEN_TO_NDC_Y(offsetPos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(offsetPos.x), PUBLIC_SCREEN_TO_NDC_Y(offsetPos.y)),
		vec2d(PUBLIC_SCREEN_TO_NDC_X(offsetPos.x), PUBLIC_SCREEN_TO_NDC_Y(pos.y))
	};
}

void rectangle::bindCtx(WFSGL* ctx) {
	this->ctx = ctx;
}

bool rectangle::intersects(rectangle a, rectangle b) {
	vec2d posA = a.getPos();
	vec2d posB = b.getPos();

	vec2d ofsposA = a.getOffsetPos();
	vec2d ofsposB = b.getOffsetPos();
	return posA.x < posB.x + ofsposB.x &&
		posA.x + ofsposA.x > posB.x &&
		posA.y < posB.y + ofsposB.y &&
		ofsposA.y + posA.y > posB.y;
}

bool rectangle::contains(rectangle a, rectangle b) {
	vec2d posA = a.getPos();
	vec2d posB = b.getPos();

	vec2d ofsposA = a.getOffsetPos();
	vec2d ofsposB = b.getOffsetPos();

	return (posB.x + ofsposB.x) < (posA.x + ofsposA.x)
		&& posB.x > posA.x
		&& posB.y > posA.y
		&& posB.y + ofsposB.y < posA.y + ofsposA.y;
}

void rectangle::fromTransform(transform *tr) {
	this->square = tr->square;
	this->setPos(tr->getPos());
	this->setOffsetPos(vec2d(tr->getWidth(), tr->getHeight()));
}

vec2d rectangle::getPos() { return this->pos; }
vec2d rectangle::getOffsetPos() { return this->offsetPos; }

void rectangle::setPos(vec2d pos) { this->pos = pos; }
void rectangle::setOffsetPos(vec2d offsetPos) { this->offsetPos = offsetPos; }
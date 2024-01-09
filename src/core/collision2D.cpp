#include "definitions.h"

object* collision2D::getGameObject() {
	return this->gameObject;
}

collisionType collision2D::getCollisionType() {
	return this->colType;
}

void collision2D::setGameObject(object* obj) {
	this->gameObject = obj;
}

void collision2D::setCollisionType(collisionType col) {
	this->colType = col;
}
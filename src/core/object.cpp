#include "definitions.h"

object::object(int layer, ObjectType objType) {
	this->add_component<transform>();
	this->layer = layer;
	this->objType = objType;
}

void object::update() {
	for (auto component : components) {
		component.second->update();
	}
}

object object::gameObject() { return *this; }

int object::get_layer() {
	return this->layer;
}
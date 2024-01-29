#include "definitions.h"

object::object(const char* name, int layer, ObjectType objType) {
	this->add_component<transform>();
	this->layer = layer;
	this->objType = objType;
	this->name = std::string(name);
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

void object::set_layer(int layer) {
	this->layer = layer;
}
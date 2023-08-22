#ifndef DOG_ENGINE_OBJECT
#define DOG_ENGINE_OBJECT

#include <vector>
#include "component.h"
#include "../lib/helper.h"
#include "../lib/exception.h"
#include "sprite.h"
#include "../../ext/include/wfsgl32.h"
#include <unordered_map>
#include "transform.h"

class object {
private:
	std::unordered_map<std::string, base_component*> components;
	int width, height;
	int layer;

public:
	object() : width(0), height(0), layer(0) { }

	object(int width, int height, vec2d pos, int layer = 0) {
		this->width = width;
		this->height = height;
		this->add_component<transform>(transform(pos - vec2d(width / 2, height / 2)));
		this->layer = layer;
	}

	transform *get_transfrom() {
		auto it = components.find(typeid(transform).name());
		if (it != components.end()) {
			return dynamic_cast<transform*>(it->second);
		}
		return nullptr;
	}

	virtual void update() {
		for (auto component : components) {
			component.second->update();
		}
	}

	object gameObject() { return *this; }

	int get_layer() {
		return this->layer;
	}

	template<typename T>
	T* get_component() {
		auto it = components.find(typeid(T).name());
		if (it != components.end()) {
			return dynamic_cast<T*>(it->second);
		}
		return nullptr;
	}

	template<typename T, typename... Args>
	T* add_component(Args&&... args) {
		T* component = new T(std::forward<Args>(args)...);
		components[typeid(T).name()] = component;
		return component;
	}
};

#endif
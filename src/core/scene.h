#ifndef DOG_ENGINE_SCENE2D
#define DOG_ENGINE_SCENE2D

#include "../../ext/include/wfsgl32.h"
#include "sprite.h"
#include "object.h"

class scene {
private:
	std::vector<object> objects;
	std::vector<Pixel> screenBuffer;

	bool sortByLayers() {
		if (objects.size() <= 1)
			return false;

		for (int i = 0; i < objects.size(); i++) {
			for (int j = 0; j < objects.size(); j++) {
				if (j == 0)
					continue;
				if (objects[j].get_layer() < objects[j - 1].get_layer())
					std::swap(objects[j], objects[j-1]);
			}
		}
		return true;
	}
public:

	scene() {
		objects = std::vector<object>();
		screenBuffer = std::vector<Pixel>();
	}

	void renderScene() {
		mainCamera.update();
		sortByLayers();
#pragma omp parallel for
		for (object a : objects) {
			a.update();
		}
	}

	void addObject(object obj) {
		objects.push_back(obj);
	}
};

#endif
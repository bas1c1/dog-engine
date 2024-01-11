#include "definitions.h"

scene::scene() {

}

bool scene::sortByLayers() {
	if (objects.size() <= 1)
		return false;

	for (int i = 0; i < objects.size(); i++) {
		for (int j = 0; j < objects.size(); j++) {
			if (j == 0)
				continue;
			if (objects[j]->get_layer() < objects[j - 1]->get_layer())
				std::swap(objects[j], objects[j - 1]);
		}
	}
	return true;
}

void scene::create_scene(camera *mainCam) {
	objects = std::vector<object*>();
	screenBuffer = std::vector<Pixel>();
	this->mainCam = mainCam;
}

void scene::renderScene() {
	sortByLayers();
	glPushMatrix();
	mainCam->update();
	for (object *a : objects) {
		a->update();
	}
	glPopMatrix();
}

std::vector<std::string> scene::getObjectList() {
	std::vector<std::string> tmp = std::vector<std::string>();
	for (object *a : objects) {
		tmp.push_back(a->name);
	}
	return tmp;
}

void scene::addObject(object *obj) {
	objects.push_back(obj);
}

object *scene::getObjectByName(std::string name) {
	for (object *obj : objects) {
		if (obj->name == name)
			return obj;
	}
	return nullptr;
}

object *scene::getObject(int index)
{
	return objects[index];
}

std::vector<object*> scene::getObjects()
{
	return objects;
}

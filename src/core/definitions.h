#ifndef DOG_ENGINE_DEFINITIONS
#define DOG_ENGINE_DEFINITIONS

#define SCREEN_TO_NDC_X(x) (x / ctx.width * 2 - 1)
#define SCREEN_TO_NDC_Y(y) (y / ctx.height * 2 - 1)

#define writeSprite(square) glBegin(GL_QUADS);\
								 glTexCoord2f(0, 1);\
								 glVertex2f(square[0].x, square[0].y);\
								 glTexCoord2f(0, 0);\
								 glVertex2f(square[1].x, square[1].y);\
								 glTexCoord2f(1, 0);\
								 glVertex2f(square[2].x, square[2].y);\
								 glTexCoord2f(1, 1);\
								 glVertex2f(square[3].x, square[3].y);\
								 glEnd();

#define FASTPROJECTX(x, width, camx)  x - camx + width /  2
#define FASTPROJECTY(y, height, camy) y - camy + height / 2

#include <vector>
#include "../lib/wfsgl32.h"
#include <unordered_map>
#include "component.h"

class camera;
class object;
class sprite;
class transform;
class scene;

class object {
private:
	std::unordered_map<std::string, base_component*> components;
	int layer;
public:
	ObjectType objType;
	object(int layer = 0, ObjectType objType = ObjectType::BASE);
	virtual void update();
	object gameObject();
	int get_layer();
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

class camera : object {
private:
	vec2d uv;
public:
	vec2d pos;
	camera();
	camera(int width, int height, vec2d pos, WFSGL& ctx);
	void update() override;
	object gameObject();
	vec2d project(vec2d point);
};

class transform : public base_component {
private:
	int width;
	int height;
	vec2d pos;
	vec2d scale;
	float rotationAngle;
	WFSGL ctx;

	void rotateObj(float rotationAngle);
	void scaleObj(vec2d scale);
public:
	std::vector<vec2d> square;

	transform();
	transform(int width, int height, vec2d pos, WFSGL& ctx, ObjectType objType = ObjectType::BASE, vec2d localScale = vec2d(1, 1), float rotationAngle = 0);
	void update() override;
	void setPosition(vec2d newPos);
	void moveToPos(vec2d newPos);

	vec2d getPos();
	vec2d getScale();
	float getRotationAngle();

	void setPos(vec2d pos);
	void setScale(vec2d scale);
	void setRotationAngle(float rotationAngle);
};

class sprite : public base_component {
private:
	WFSGL ctx;
	std::vector<Pixel> pixels;
	int width;
	int height;
	vec2d localPos;
	vec2d localScale;
	float localRotationAngle;

	camera cam2d;
	GLuint gpuTexture;
	transform* tr;
public:

	sprite();
	sprite(int width, int height, camera& cam2d);
	sprite(int width, int height, std::vector<Pixel> pixels, camera& cam2d);
	sprite(const char* filename, int width, int height, camera& cam2d);
	void bindTransform(transform* tr);
	void update() override;
	void GenerateGPUTexture();
	bool FillArea(vec2d f, vec2d s, Pixel color);
};

class scene {
private:
	std::vector<object> objects;
	std::vector<Pixel> screenBuffer;
	camera mainCam;

	bool sortByLayers();
public:
	scene(camera &mainCam);
	void renderScene();
	void addObject(object obj);
	object getObject(int index);
};

#endif
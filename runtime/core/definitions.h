#ifndef DOG_ENGINE_DEFINITIONS
#define DOG_ENGINE_DEFINITIONS

#define SCREEN_TO_NDC(v) (vec2d(v.x / WFSGL::nWindowWidth * 2 - 1, v.y / WFSGL::nWindowHeight * 2 - 1))
#define PUBLIC_SCREEN_TO_NDC(v) (vec2d(v.x / WFSGL::nWindowWidth * 2 - 1, v.y / WFSGL::nWindowHeight * 2 - 1))

#define PUBLIC_SCREEN_TO_NDC_X(x) (x / WFSGL::nWindowWidth * 2 - 1)
#define PUBLIC_SCREEN_TO_NDC_Y(y) (y / WFSGL::nWindowHeight * 2 - 1)

#define SCREEN_TO_NDC_X(x) (x / WFSGL::nWindowWidth * 2 - 1)
#define SCREEN_TO_NDC_Y(y) (y / WFSGL::nWindowHeight * 2 - 1)

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

#define FASTPROJECTX(x, width)  x + width /  2
#define FASTPROJECTY(y, height) y + height / 2

#include <vector>
#include <unordered_map>
#include "../lib/objectType.h"
#include "../lib/wfsgl32.h"
#include "../lib/vec.h"
#include "../lib/shaders.h"

class camera;
class object;
class sprite;
class animation;
class spriteRender;
class rigidbody2D;
class collision2D;
class box_collider2D;
class transform;
class scene;
class spriteBatching;
class particleGenerator;

class Quaternion {
public:
	float w, x, y, z = 0.0f;
	Quaternion(float w, float x, float y, float z)
		: w(w), x(x), y(y), z(z) {}

	Quaternion operator*(const Quaternion& other) const {
		return Quaternion(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w
		);
	}

	Quaternion conjugate() const {
		return Quaternion(w, -x, -y, -z);
	}
};

struct AABB
{
	vec2d min;
	vec2d max;
};

class base_component {
public:
	object* gameObj;
	base_component() = default;
	virtual void update() {};
	virtual std::string toString() { return std::string("default component"); };
};


class object {
private:
	std::unordered_map<std::string, base_component*> components;
	int layer;
	
public:
	std::string name;
	ObjectType objType;
	object(const char *name = "Unnamed", int layer = 0, ObjectType objType = ObjectType::BASE);
	virtual void update();
	object gameObject();
	int get_layer();
	void set_layer(int layer);

	std::vector<base_component*> getComponentsList() {
		std::vector<base_component*> tmp = std::vector<base_component*>();
		for (auto& it : components) {
			tmp.push_back(it.second);
		}
		return tmp;
	}

	template<typename T>
	T* get_component() {
		auto it = components.find(typeid(T).name());
		if (it != components.end()) {
			return dynamic_cast<T*>(it->second);
		}
		return nullptr;
	}

	template<typename T>
	bool has_component() {
		auto it = components.find(typeid(T).name());
		if (it != components.end()) {
			return true;
		}
		return false;
	}

	template<typename T, typename... Args>
	T* add_component(Args&&... args) {
		T* component = new T(std::forward<Args>(args)...);
		components[typeid(T).name()] = component;
		components[typeid(T).name()]->gameObj = this;
		return component;
	}
};

class camera : object {
private:
	vec2d uv;
	float zoom = 1.f;
	vec2d pos;
public:
	camera();
	camera(int width, int height, vec2d pos);
	void update() override;
	object gameObject();
	vec2d project(vec2d point);

	vec2d getUV();
	float getZoom();
	vec2d getPos();

	void setUV(vec2d uv);
	void setZoom(float zoom);
	void setPos(vec2d pos);
};

class transform : public base_component {
private:
	int width;
	int height;
	vec2d pos;
	vec2d scale;
	float rotationAngle;

	void rotateObj(float rotationAngle);
	void scaleObj(vec2d scale);
	ObjectType objType;
public:
	std::vector<vec2d> square;
	std::vector<vec2d> drawable_square;

	transform();
	transform(int width, int height, int posx, int posy, ObjectType objType);
	transform(int width, int height, vec2d pos, ObjectType objType = ObjectType::BASE, vec2d localScale = vec2d(1, 1), float rotationAngle = 0);
	void update() override;
	std::string toString() override;
	void setPosition(vec2d newPos);
	void moveToPos(vec2d newPos);

	vec2d getPos();
	vec2d getScale();
	float getRotationAngle();
	int getWidth();
	int getHeight();
	ObjectType getObjType();

	void setPos(vec2d pos);
	void setScale(vec2d scale);
	void setRotationAngle(float rotationAngle);
	void setWidth(int width);
	void setHeight(int height);
	void setObjType(ObjectType objType);
};

enum class collisionType {
	BOX_COLLIDER_COLLISON2D,
	OTHER_COLLISION2D,
	DEFAULT_COLLISION
};

class collision2D {
public:
	object *gameObject;
	collisionType colType;

	collision2D(object *gameObject = nullptr, collisionType colType= collisionType::DEFAULT_COLLISION) {
		this->gameObject = gameObject;
		this->colType = colType;
	}

	object* getGameObject();
	collisionType getCollisionType();

	void setGameObject(object* obj);
	void setCollisionType(collisionType col);
};

class box_collider2D : public base_component {
private:
	transform* tr;
	scene* sc;
	std::vector<collision2D> collision = std::vector<collision2D>();

public:
	void bindTransform(transform* tr);
	void bindScene(scene *sc);
	bool detectCollision(transform* obj);
	void update() override;
	std::string toString() override;
	std::vector<collision2D> getAllCollisions();
	collision2D getCollision(int index=0);
};

enum class rigidbodyType {
	DYNAMIC = 1,
	STATIC = 2,
	KINEMATIC = 3
};

class rigidbody2D : public base_component {
private:
	transform* tr;
	scene* sc;
	rigidbodyType rbType = rigidbodyType::DYNAMIC;
	
	float impulse = 0.0f;
	float maxImpulse = 10.f;
	int mass = 1;
	float gravity = -9.81f;
	vec2d velocity = vec2d(0, 0);

public:
	
	void bindTransform(transform* tr);
	void bindScene(scene* sc);
	void update() override;
	std::string toString() override;

	void applyForce(vec2d force);
	int resolveCollision(rigidbody2D* other);

	void setVelocity(vec2d velocity);
	vec2d getVelocity();
	void setGravity(float gravity);
	float getGravity();
	void setMass(int mass);
	int getMass();
	void setType(rigidbodyType rbType);
	rigidbodyType getType();
};

class sprite {
public:
	std::vector<Pixel> pixels;
	int width;
	int height;
	GLuint gpuTexture;

	std::string filename;
	sprite();
	sprite(int width, int height);
	sprite(int width, int height, std::vector<Pixel> pixels);
	sprite(std::string filename);
	void fromFile(std::string filename);
};

class spriteRender : public base_component {
private:
	transform* tr;
	sprite* sp;
public:
	spriteRender();
	void bindTransform(transform* tr);
	void bindSprite(sprite* sp);
	sprite* getSprite();
	void update() override;
	std::string toString() override;
};

struct spriteFrame {
	int height, width, x, y;
};

struct particleFrame {
	vec2d pos, velocity, size;
	vec4d col;
	float life;

	particleFrame() : life(0.0f) { col = vec4d(1, 1, 1, 1); pos = vec2d(0, 0); velocity = vec2d(0, 0); size = vec2d(0, 0); }
};

class spriteBatching {
private:
	GLuint VBO, VAO, EBO;
	std::vector<spriteFrame> frames;
	std::vector<GLfloat> vaos;
	GLuint texture;

public:
	spriteBatching();

	void init();

	void render();

	void clearBuffers();
	GLuint getTexture();
	void setTexture(GLuint texture);
	std::vector<spriteFrame> getFrames();
	spriteFrame getFrame(int index);
	void pushFrame(spriteFrame frame);
	void setFrame(int index, spriteFrame frame);
};

class particleGenerator : public base_component {
private:
	std::vector<particleFrame> particles;
	int amount;
	GLuint texture;

	spriteBatching spb;

	vec2d pos;
	vec2d defaultSize;
	vec2d defaultVelocity;
	float defaultLifetime;
public:
	std::string filename;
	particleGenerator();
	particleGenerator(std::string, int amount, vec2d defaultSize, vec2d defaultVelocity, float defaultLifetime, vec2d pos);

	void update() override;
	std::string toString() override;

	vec2d getPos();
	vec2d getDefaultSize();
	vec2d getDefaultVelocity();
	float getDefaultLifetime();
	std::string getFilename();
	int getAmount();

	void setPos(vec2d pos);
	void setDefaultSize(vec2d defaultSize);
	void setDefaultVelocity(vec2d defaultVelocity);
	void setDefaultLifetime(float defaultLifetime);
	void setTexture(std::string texture);
	void setAmount(int amount);
};

class animation : public base_component {
private:
	transform* tr;
	std::vector<sprite*> sps;
	int delay = 1;
public:
	animation();
	animation(int delay);
	void bindTransform(transform* tr);
	void addSprite(sprite* sp);
	void update() override;
	std::string toString() override;

	int getDelay();
	void setDelay(int delay);
	std::vector<sprite*> getAllFrames();
};

class scene {
private:
	std::vector<object*> objects;
	std::vector<Pixel> screenBuffer;
	camera *mainCam;

	bool sortByLayers();
public:
	scene();
	void create_scene(camera *mainCam);
	void renderScene();
	void addObject(object *obj);
	object *getObject(int index);
	object *getObjectByName(std::string name);
	std::vector<std::string> getObjectList();
	std::vector<object*> getObjects();
};

#endif
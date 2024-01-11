#include "lua_scripting.h"
#include "../lib/vec.h"

sol::state lua{};

template<typename T>
auto Wrap(T& reference) {
	return [&](std::variant<T, sol::lua_nil_t> value) -> sol::optional<T> {
		if (const T* pValue = std::get_if<T>(&value)) {
			reference = *pValue;
			return {};
		}
		else {
			return reference;
		}
	};
}

extern float deltaTime;
extern scene mainScene;
extern camera mainCamera;

scene *getMainScene() {
	return &mainScene;
}

float getDeltaTime() {
	return deltaTime;
}

/*object getObject(object* obj) {
	return *obj;
}*/

vec2d newVec2d(float x, float y) {
	return vec2d(x, y);
}

vec3d newVec3d(float x, float y, float z) {
	return vec3d(x, y, z);
}

transform* addTransformComponent() {

}

void dlua::init() {
	lua.open_libraries(sol::lib::base);

	sol::usertype<vec2d> vec2d_type = lua.new_usertype<vec2d>("vec2d", sol::constructors<vec2d(), vec2d(float, float)>());
	vec2d_type["x"] = sol::property(&vec2d::getX, &vec2d::setX);
	vec2d_type["y"] = sol::property(&vec2d::getY, &vec2d::setY);

	sol::usertype<vec3d> vec3d_type = lua.new_usertype<vec3d>("vec3d", sol::constructors<vec3d(), vec3d(float, float, float), vec3d(vec2d)>());
	vec3d_type["x"] = sol::property(&vec3d::getX, &vec3d::setX);
	vec3d_type["y"] = sol::property(&vec3d::getY, &vec3d::setY);
	vec3d_type["z"] = sol::property(&vec3d::getZ, &vec3d::setZ);

	lua["objectType"] = lua.create_table_with(
		"ANOTHER", ObjectType::ANOTHER,
		"BASE", ObjectType::BASE,
		"CAMERA", ObjectType::CAMERA
	);

	sol::usertype<object> obj_type = lua.new_usertype<object>("object", sol::constructors<object(), object(const char*, int, ObjectType)>());
	obj_type["layer"] = sol::property(&object::get_layer, &object::set_layer);
	obj_type["add_transform"] = &object::add_component<transform, int, int, int, int, ObjectType>;
	obj_type["add_box_collider2D"] = &object::add_component<box_collider2D>;
	obj_type["add_rigidbody2D"] = &object::add_component<rigidbody2D>;
	//obj_type["add_sprite"] = &object::add_component<sprite, std::string>;
	obj_type["add_spriteRender"] = &object::add_component<spriteRender>;
	obj_type["add_animation"] = &object::add_component<animation>;

	obj_type["get_transform"] = &object::get_component<transform>;
	obj_type["get_box_collider2D"] = &object::get_component<box_collider2D>;
	obj_type["get_rigidbody2D"] = &object::get_component<rigidbody2D>;
	//obj_type["get_sprite"] = &object::get_component<sprite>;
	obj_type["get_spriteRender"] = &object::get_component<spriteRender>;
	obj_type["get_animation"] = &object::get_component<animation>;

	obj_type["get_gameObject"] = &object::gameObject;

	obj_type["get_component_list"] = &object::getComponentsList;

	sol::usertype<camera> camera_type = lua.new_usertype<camera>("camera", sol::constructors<camera(), camera(int, int, vec2d)>());
	camera_type["uv"] = sol::property(&camera::getUV, &camera::setUV);
	camera_type["zoom"] = sol::property(&camera::getZoom, &camera::setZoom);
	camera_type["pos"] = sol::property(&camera::getPos, &camera::setPos);

	camera_type["gameObject"] = &camera::gameObject;
	camera_type["project"] = &camera::project;

	sol::usertype<transform> transform_type = lua.new_usertype<transform>("transform", sol::constructors<transform(), transform(int, int, vec2d, ObjectType, vec2d, float)>());
	transform_type["toString"] = &transform::toString;
	transform_type["setPosition"] = &transform::setPosition;
	transform_type["moveToPos"] = &transform::moveToPos;
	transform_type["gameObject"] = &transform::gameObj;

	transform_type["pos"] = sol::property(&transform::getPos, &transform::setPos);
	transform_type["scale"] = sol::property(&transform::getScale, &transform::setScale);
	transform_type["rotationAngle"] = sol::property(&transform::getRotationAngle, &transform::setRotationAngle);
	transform_type["width"] = sol::property(&transform::getWidth, &transform::setWidth);
	transform_type["height"] = sol::property(&transform::getHeight, &transform::setHeight);

	lua["collisionType"] = lua.create_table_with(
		"BOX_COLLIDER_COLLISON2D", collisionType::BOX_COLLIDER_COLLISON2D,
		"OTHER_COLLISION2D", collisionType::OTHER_COLLISION2D,
		"DEFAULT_COLLISION", collisionType::DEFAULT_COLLISION
	);

	lua["rigidbodyType"] = lua.create_table_with(
		"DYNAMIC", rigidbodyType::DYNAMIC,
		"KINEMATIC", rigidbodyType::KINEMATIC,
		"STATIC", rigidbodyType::STATIC
	);

	sol::usertype<collision2D> collision2D_type = lua.new_usertype<collision2D>("collision2D", sol::constructors<collision2D(), collision2D(object*, collisionType)>());
	collision2D_type["object"] = sol::property(&collision2D::getGameObject, &collision2D::setGameObject);
	collision2D_type["colType"] = sol::property(&collision2D::getCollisionType, &collision2D::setCollisionType);

	sol::usertype<box_collider2D> box_collider2D_type = lua.new_usertype<box_collider2D>("box_collider2D", sol::constructors<box_collider2D()>());
	box_collider2D_type["gameObject"] = &box_collider2D::gameObj;
	box_collider2D_type["toString"] = &box_collider2D::toString;

	box_collider2D_type["bindTransform"] = &box_collider2D::bindTransform;
	box_collider2D_type["bindScene"] = &box_collider2D::bindScene;

	box_collider2D_type["getAllCollisions"] = &box_collider2D::getAllCollisions;
	box_collider2D_type["getCollision"] = &box_collider2D::getCollision;

	sol::usertype<rigidbody2D> rigidbody2D_type = lua.new_usertype<rigidbody2D>("rigidbody2D", sol::constructors<rigidbody2D()>());
	rigidbody2D_type["gameObject"] = &rigidbody2D::gameObj;
	rigidbody2D_type["toString"] = &rigidbody2D::toString;

	rigidbody2D_type["bindTransform"] = &rigidbody2D::bindTransform;
	rigidbody2D_type["bindScene"] = &rigidbody2D::bindScene;
	rigidbody2D_type["applyForce"] = &rigidbody2D::applyForce;

	rigidbody2D_type["velocity"] = sol::property(&rigidbody2D::setVelocity, &rigidbody2D::getVelocity);
	rigidbody2D_type["gravity"] = sol::property(&rigidbody2D::setGravity, &rigidbody2D::getGravity);
	rigidbody2D_type["mass"] = sol::property(&rigidbody2D::setMass, &rigidbody2D::getMass);
	rigidbody2D_type["type"] = sol::property(&rigidbody2D::getType, &rigidbody2D::setType);

	sol::usertype<sprite> sprite_type = lua.new_usertype<sprite>("sprite", sol::constructors<sprite(), sprite(int, int), sprite(int, int, std::vector<Pixel>), sprite(std::string)>());
	sprite_type["fillArea"] = &sprite::FillArea;
	sprite_type["fromFile"] = &sprite::fromFile;

	sol::usertype<animation> animation_type = lua.new_usertype<animation>("animation", sol::constructors<animation()>());
	animation_type["bindTransform"] = &animation::bindTransform;
	animation_type["addSprite"] = &animation::addSprite;
	animation_type["delay"] = sol::property(&animation::getDelay, &animation::setDelay);

	sol::usertype<spriteRender> spriteRender_type = lua.new_usertype<spriteRender>("spriteRender", sol::constructors<spriteRender()>());
	spriteRender_type["gameObject"] = &spriteRender::gameObj;
	spriteRender_type["bindTransform"] = &spriteRender::bindTransform;
	spriteRender_type["bindSprite"] = &spriteRender::bindSprite;

	sol::usertype<scene> scene_type = lua.new_usertype<scene>("scene", sol::constructors<scene()>());
	scene_type["createScene"] = &scene::create_scene;
	scene_type["getObjects"] = &scene::getObjects;
	scene_type["addObject"] = &scene::addObject;
	scene_type["getObject"] = &scene::getObject;
	scene_type["getObjectByName"] = &scene::getObjectByName;
	scene_type["getObjectList"] = &scene::getObjectList;
	scene_type["renderScene"] = &scene::renderScene;

	lua["deltaTime"] = &getDeltaTime;
	lua["mainScene"] = &getMainScene;
	lua["mainCamera"] = []() {return &mainCamera; };

	lua["vec2d"] = &newVec2d;
	lua["vec3d"] = &newVec3d;

	lua["getKey"] = [](char ch) { return (bool)GetAsyncKeyState(ch); };
	lua["cameraRef"] = [] (camera& cam) { return &cam; };
	lua["objectRef"] = [] (object& obj) { return &obj; };
	lua["spriteRef"] = [] (sprite& sp)  { return &sp; };
}

void dlua::run(std::string s) {
	lua.script_file(s.c_str()); 
}
#ifndef DOG_ENGINE_COMPONENTS
#define DOG_ENGINE_COMPONENTS

class base_component {
public:
	base_component() = default;
	virtual void update() {};
};

#endif
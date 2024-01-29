#ifndef WSFGL_SHADERS
#define WSFGL_SHADERS

#include "vec.h"

class custom_shader {
private:
	const char* vs;
	const char* fs;
	unsigned int prog;
public:
	custom_shader();
	custom_shader(const char* vs, const char* fs);
	void use();
	void set1f(const char* name, float a);
	void setVec2f(const char* name, vec2d a);
	void setVec3f(const char* name, vec3d a);
	void setVec4f(const char* name, vec4d a);
};

#endif
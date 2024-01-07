#pragma once
#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include "definitions.h"

namespace dlua {
	void init();
	void run(std::string s);
}
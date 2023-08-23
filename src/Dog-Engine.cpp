#include "../ext/include/wfsgl32.h"
#include <iostream>
#include "core/scene.h"

#define WIDTH 1680
#define HEIGHT 1050

using namespace std::chrono;

std::string s = std::string("Dog Engine GL test");
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();
WFSGL wfsgl(WIDTH, HEIGHT, sw, 1);

scene m;

void on_start() {
    mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0), wfsgl);

    object game_object = object(800, 200, vec2d(0, 0), 0);
    game_object.add_component<sprite>("sprites/800x200.png", wfsgl, 800, 200, 0, 0);

    m.addObject(game_object);
}

void mainloop() {
    if (GetAsyncKeyState(VK_RIGHT)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(150 * wfsgl.deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(-150 * wfsgl.deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_UP)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(0, -150 * wfsgl.deltaTime));
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(0, 150 * wfsgl.deltaTime));
    }

    m.renderScene();
}

int main(int argc, char* argv[])
{
	wfsgl.WFSGLCreateWindow();
    
	wfsgl.WFSGLStartEngine();
	return 0;
}
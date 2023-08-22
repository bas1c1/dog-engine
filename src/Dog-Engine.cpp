#include "../ext/include/wfsgl32.h"
#include <iostream>
#include "core/scene.h"
#include "core/object.h"

#define WIDTH 800
#define HEIGHT 600

using namespace std::chrono;

std::string s = std::string("Dog Engine GL test");
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();
WFSGL wfsgl(WIDTH, HEIGHT, sw, 1);

scene m;

void on_start() {
    mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0));
    
    object game_object = object(800, 200, vec2d(0, 0), 0);
    game_object.add_component<sprite>(wfsgl, 800, 200, 0, 0);
    game_object.get_component<sprite>()->fromFile("sprites/800x200.png", wfsgl, 800, 200, 0, 0);

    object game_object1 = object(800, 200, vec2d(0, 100), 1);
    game_object1.add_component<sprite>(wfsgl, 800, 200, 0, 0);
    game_object1.get_component<sprite>()->fromFile("sprites/800x200.png", wfsgl, 800, 200, 0, 300);

    object game_object2 = object(800, 200, vec2d(0, 200), 2);
    game_object2.add_component<sprite>(wfsgl, 800, 200, 0, 0);
    game_object2.get_component<sprite>()->fromFile("sprites/800x200.png", wfsgl, 800, 200, 0, 600);

    object game_object3 = object(800, 200, vec2d(0, 300), 3);
    game_object3.add_component<sprite>(wfsgl, 800, 200, 0, 0);
    game_object3.get_component<sprite>()->fromFile("sprites/800x200.png", wfsgl, 800, 200, 0, 900);

    m.addObject(game_object);
    m.addObject(game_object1);
    m.addObject(game_object2);
    m.addObject(game_object3);
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
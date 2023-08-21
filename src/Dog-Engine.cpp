#include <iostream>
#include "core/scene.h"
#include "core/object.h"

using namespace std::chrono;

std::string s = std::string("Dog Engine GL test");
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();
WFSGL wfsgl(800, 600, sw, 1);

scene m;

void on_start() {
    mainCamera = camera(800, 600, vec2d(0, 0));
    
    object game_object = object(800, 200, vec2d(0, 0), 0);
    game_object.add_component<sprite>(wfsgl, 800, 200, 0, 0);
    game_object.get_component<sprite>()->fromFile("800x200.png", wfsgl, 800, 200, 0, 0);
    m.addObject(game_object);
}

void mainloop() {
    if (GetAsyncKeyState(VK_RIGHT)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(1, 0));
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(-1, 0));
    }

    if (GetAsyncKeyState(VK_UP)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(0, -1));
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        mainCamera.gameObject().get_transfrom()->moveToPos(vec2d(0, 1));
    }

    wfsgl.WFSGLFill(Pixel(0, 0, 0));
    m.renderScene();
}

int main(int argc, char* argv[])
{
	wfsgl.WFSGLCreateWindow();
	wfsgl.WFSGLStartEngine();
	return 0;
}
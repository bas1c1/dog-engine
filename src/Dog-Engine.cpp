#define WIDTH 1680
#define HEIGHT 1050

#include "lib/wfsgl32.h"
#include <iostream>
#include "core/definitions.h"

std::string s = std::string("Dog Engine GL test");
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();

WFSGL globalCtx(WIDTH, HEIGHT, sw);

using namespace std::chrono;

camera mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0), globalCtx);
scene m(mainCamera);

object game_object;
object game_object1;

void on_start() {
    game_object = object(1);
    game_object.add_component<transform>(800, 200, vec2d(0, 0), globalCtx);
    game_object.add_component<sprite>("sprites/800x200.png", 800, 200, mainCamera);
    game_object.get_component<sprite>()->bindTransform(game_object.get_component<transform>());

    m.addObject(game_object);

    game_object1 = object(0);
    game_object1.add_component<transform>(800, 200, vec2d(0, 0), globalCtx);
    game_object1.add_component<sprite>(800, 200, mainCamera);
    game_object1.get_component<sprite>()->bindTransform(game_object1.get_component<transform>());
    game_object1.get_component<sprite>()->FillArea(vec2d(0,0), vec2d(800, 200), Pixel(255, 0, 0));

    m.addObject(game_object1);
}

void mainloop() {
    if (GetAsyncKeyState(VK_RIGHT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(150 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(-150 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_UP)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, 150 * deltaTime));
        game_object1.get_component<transform>()->moveToPos(vec2d(150 * deltaTime * 2, 0));
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, -150 * deltaTime));
        game_object1.get_component<transform>()->moveToPos(vec2d(-(150 * deltaTime * 2), 0));
    }

    game_object1.get_component<transform>()->setScale(vec2d(1.5f, 2));

    globalCtx.WFSGLFill(Pixel(0, 0, 0));

    m.renderScene();
}

int main(int argc, char* argv[])
{
    globalCtx.WFSGLCreateWindow();
    
    globalCtx.WFSGLStartEngine();
	return 0;
}
#define WIDTH 1680
#define HEIGHT 1050

#pragma comment(lib, "d3d9.lib")

#include "lib/wfsgl32.h"
#include <iostream>
#include "core/definitions.h"
#include "core/gui.h"
#include <thread>

std::string s = std::string("Dog Engine GL test");
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();

WFSGL globalCtx(WIDTH, HEIGHT, sw);

float cameraZoom = 1.f;

using namespace std::chrono;

camera mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0), globalCtx);
//scene m(&mainCamera);

object game_object;
object game_object1;

extern scene mainScene;

void on_start() {
    mainScene.create_scene(&mainCamera);

    game_object = object("obj1", 1);
    game_object.add_component<transform>(800, 200, vec2d(0, 0), globalCtx);
    game_object.add_component<box_collider2D>();
    game_object.get_component<box_collider2D>()->bindTransform(game_object.get_component<transform>());
    game_object.get_component<box_collider2D>()->bindScene(&mainScene);
    game_object.add_component<sprite>("sprites/800x200.png", mainCamera);
    game_object.get_component<sprite>()->bindTransform(game_object.get_component<transform>());

    mainScene.addObject(&game_object);

    game_object1 = object("obj2", 0);
    game_object1.add_component<transform>(800, 200, vec2d(802, 0), globalCtx);
    //game_object1.get_component<transform>()->setScale(vec2d(2, 2));
    game_object1.add_component<box_collider2D>();
    game_object1.get_component<box_collider2D>()->bindTransform(game_object1.get_component<transform>());
    game_object1.get_component<box_collider2D>()->bindScene(&mainScene);
    game_object1.add_component<sprite>(800, 200, mainCamera);
    game_object1.get_component<sprite>()->bindTransform(game_object1.get_component<transform>());
   

    mainScene.addObject(&game_object1);
}

void mainloop() {
    dgui::BeginRender();
    dgui::Render();
    dgui::EndRender();

    if (GetAsyncKeyState(VK_RIGHT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(150 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(-150 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_UP)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, 150 * deltaTime));
        //game_object1.get_component<transform>()->moveToPos(vec2d(150 * deltaTime * 2, 0));
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, -150 * deltaTime));
        //game_object1.get_component<transform>()->moveToPos(vec2d(-(150 * deltaTime * 2), 0));
    }

    if (GetAsyncKeyState('S')) {
        cameraZoom += 1 * deltaTime;
        mainCamera.zoom = cameraZoom;
    }

    if (GetAsyncKeyState('W')) {
        cameraZoom -= 1 * deltaTime;
        mainCamera.zoom = cameraZoom;
    }

    globalCtx.WFSGLFill(Pixel(0, 0, 0));

    mainScene.renderScene();

    collision2D col = game_object.get_component<box_collider2D>()->getCollision();

    if(col.gameObject)
        std::cout << col.gameObject->name << std::endl;
}

int main(int argc, char* argv[])
{
    SetConsoleTitle(L"Debug console");

    globalCtx.WFSGLCreateWindow();

    dgui::CreateHWindow((char*)"Dog Engine 2D");
    dgui::CreateDevice();
    dgui::CreateImGui();
    
    globalCtx.WFSGLStartEngine();

    dgui::DestroyImGui();
    dgui::DestroyDevice();
    dgui::DestroyHWindow();
	return 0;
}
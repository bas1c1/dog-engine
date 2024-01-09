﻿#define WIDTH 1680
#define HEIGHT 1050

#pragma comment(lib, "d3d9.lib")

#include "lib/wfsgl32.h"
#include <iostream>
#include "core/definitions.h"
#include "core/gui.h"
#include "core/lua_scripting.h"

LPCWSTR sw = L"Dog Engine GL test";

camera mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0));

float cameraZoom = 1.f;

using namespace std::chrono;
extern float deltaTime;
extern scene mainScene;

object game_object;
object game_object1;

void WFSGL::on_start() {
    mainScene.create_scene(&mainCamera);
    
    game_object = object("obj2", 1);
    game_object.add_component<transform>(800, 200, vec2d(0, 0));
    game_object.get_component<transform>()->setRotationAngle(0);
   
    game_object.add_component<sprite>(std::string("sprites/800x200.png"));
    game_object.get_component<sprite>()->bindTransform(game_object.get_component<transform>());

    mainScene.addObject(&game_object);
    /*
    game_object1 = object("obj2", 0);
    game_object1.add_component<transform>(800, 200, vec2d(0, -400));
    game_object1.get_component<transform>()->setScale(vec2d(1, 2));

    game_object1.add_component<box_collider2D>();
    game_object1.get_component<box_collider2D>()->bindTransform(game_object1.get_component<transform>());
    game_object1.get_component<box_collider2D>()->bindScene(&mainScene);

    game_object1.add_component<rigidbody2D>();
    game_object1.get_component<rigidbody2D>()->bindTransform(game_object1.get_component<transform>());
    game_object1.get_component<rigidbody2D>()->setMass(1);
    game_object1.get_component<rigidbody2D>()->bindScene(&mainScene);
    game_object1.get_component<rigidbody2D>()->setType(rigidbodyType::STATIC);

    game_object1.add_component<sprite>(800, 200, mainCamera);
    game_object1.get_component<sprite>()->bindTransform(game_object1.get_component<transform>());
   

    mainScene.addObject(&game_object1);*/

    dlua::init();
    dlua::run("luas/start.lua");
}

void WFSGL::mainloop() {
    dgui::BeginRender();
    dgui::Render();
    dgui::EndRender();

    /*

    if (GetAsyncKeyState(VK_RIGHT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(300 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_LEFT)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(-300 * deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_UP)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, 300 * deltaTime));
    }

    if (GetAsyncKeyState(VK_DOWN)) {
        mainCamera.gameObject().get_component<transform>()->moveToPos(vec2d(0, -300 * deltaTime));
    }

    if (GetAsyncKeyState('Q')) {
        cameraZoom += 1 * deltaTime;
        mainCamera.setZoom(cameraZoom);
    }

    if (GetAsyncKeyState('E')) {
        cameraZoom -= 1 * deltaTime;
        mainCamera.setZoom(cameraZoom);
        if (cameraZoom <= 0)
            cameraZoom += 1 * deltaTime;
    }

    if (GetAsyncKeyState('S')) {
        game_object.get_component<transform>()->moveToPos(vec2d(0, -100 * deltaTime));
    }

    if (GetAsyncKeyState('W')) {
        game_object.get_component<transform>()->moveToPos(vec2d(0, 100 * deltaTime));
    }

    if (GetAsyncKeyState('A')) {
        game_object.get_component<transform>()->moveToPos(vec2d(-100 * deltaTime, 0));
    }

    if (GetAsyncKeyState('D')) {
        game_object.get_component<transform>()->moveToPos(vec2d(100 * deltaTime, 0));
    }

    WFSGL::WFSGLFill(Pixel(0, 0, 0));

    mainScene.renderScene();*/

    WFSGL::WFSGLFill(Pixel(0, 0, 0));
    
    dlua::run("luas/update.lua");
}

int main(int argc, char* argv[])
{
    WFSGL::WFSGLConstruct(WIDTH, HEIGHT, sw);
    SetConsoleTitle(L"Debug console");

    WFSGL::WFSGLCreateWindow();

    dgui::CreateHWindow((char*)"Dog Engine 2D");
    dgui::CreateDevice();
    dgui::CreateImGui();

    WFSGL::WFSGLStartEngine();

    dgui::DestroyImGui();
    dgui::DestroyDevice();
    dgui::DestroyHWindow();
	return 0;
}
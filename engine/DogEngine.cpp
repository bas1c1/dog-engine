#define WIDTH 1680
#define HEIGHT 1050

#pragma comment(lib, "DogEngineRuntime.lib")

#pragma comment(lib, "d3d9.lib")

#include "lib/wfsgl32.h"
#include <iostream>
#include "core/definitions.h"
#include "core/lua_scripting.h"
#include "lib/io.h"
#include "gui.h"

const char* sw = "Dog Engine GL test";

camera mainCamera = camera(WIDTH, HEIGHT, vec2d(0, 0));

using namespace std::chrono;
extern float deltaTime;
extern scene mainScene;

void WFSGL::on_start() {
    mainScene.create_scene(&mainCamera);

    dlua::init();
    dlua::run("luas/start.lua");
}

void WFSGL::mainloop() {
    dgui::BeginRender();
    dgui::Render();
    dgui::EndRender();

    WFSGL::WFSGLFill(Pixel(0, 0, 0));

    dlua::run("luas/update.lua");

    mainScene.renderScene();
}

int main(int argc, char* argv[])
{
    WFSGL::WFSGLConstruct(WIDTH, HEIGHT, sw);
    SetConsoleTitle(L"Debug console");

    dgui::CreateHWindow((char*)"Dog Engine 2D");
    dgui::CreateDevice();
    dgui::CreateImGui();

    WFSGL::WFSGLCreateWindow();

    WFSGL::WFSGLStartEngine();

    dgui::DestroyImGui();
    dgui::DestroyDevice();
    dgui::DestroyHWindow();
    return 0;
}
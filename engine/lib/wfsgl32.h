#pragma comment(lib, "user32.lib")		// Visual Studio Only
#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")

#ifndef WFSGL_LIB
#define WFSGL_LIB

#include <cstdint>
#include <string>
#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <chrono>
//#include "../../ext/include/GL/glew.h"
//#include "gl.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#ifndef _WIN32
#error Your system is not supported
#else
#include <Windows.h>
#endif
#include "Pixel.h"

namespace WFSGL {
    void mainloop();
    void on_start();

    typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
    extern wglSwapInterval_t* wglSwapInterval;

    extern GLFWwindow* window;

    extern const char* name;

    extern LONG nWindowWidth;
    extern LONG nWindowHeight;

    extern LONG nViewW, nViewH, nViewX, nViewY;

    extern GLfloat halfScreenWidth;
    extern GLfloat halfScreenHeight;

    extern GLuint glBuffer;

    extern float FPS;

    void WFSGLChangeNameWithFPS(float fps);
    void UpdateViewport();
    int WFSGLCreateWindow();
    void WFSGLFill(Pixel p);
    void WFSGLConstruct(int width, int height, const char* name);
    void WFSGLStartEngine();
};

#endif
#pragma comment(lib, "user32.lib")		// Visual Studio Only
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")

#ifndef WFSGL_LIB
#define WFSGL_LIB
#pragma once

#include <cstdint>
#include <string>
#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <chrono>
#include "../../ext/include/GL/glew.h"
#include "gl.h"

#ifndef _WIN32
#error Your system is not supported
#else
#include <Windows.h>
#endif
#include "Pixel.h"

void mainloop();
void on_start();

typedef BOOL(WINAPI wglSwapInterval_t) (int interval);

class WFSGL {
private:
    wglSwapInterval_t* wglSwapInterval;

    LPCWSTR name;

    LONG nWindowWidth;
    LONG nWindowHeight;

    LONG nViewW, nViewH, nViewX, nViewY;

    HDC glDeviceContext;
    HGLRC glRenderContext;

    GLfloat halfScreenWidth;
    GLfloat halfScreenHeight;

    GLuint glBuffer;

    float fSubPixelOffsetX;
    float fSubPixelOffsetY;

    float FPS;

    void WFSGLChangeNameWithFPS(float fps);

public:
    HWND hWnd;

    int width;
    int height;

    int windowScale;

    Pixel *screenPixels;

    void UpdateViewport();
    WFSGL();
    WFSGL(int width, int height, LPCWSTR name, int windowScale = 1);
    int WFSGLCreateWindow();
    static LRESULT CALLBACK WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void WFSGLFillBuffer(std::vector<Pixel> pixels);
    void WFSGLFill(Pixel p);
    bool WFSGLSetPixel(int x, int y, Pixel p);
    bool WFSGLDrawLine(int x1, int y1, int x2, int y2, Pixel p);
    void WFSGLStartEngine();
};

#endif
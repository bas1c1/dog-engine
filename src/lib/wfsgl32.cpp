#pragma once

#include "wfsgl32.h"
LPCWSTR WFSGL::name;

LONG WFSGL::nWindowWidth;
LONG WFSGL::nWindowHeight;

LONG WFSGL::nViewW, WFSGL::nViewH, WFSGL::nViewX, WFSGL::nViewY;

GLfloat WFSGL::halfScreenWidth;
GLfloat WFSGL::halfScreenHeight;

GLuint WFSGL::glBuffer;

float WFSGL::FPS;

HWND hWnd;
HDC glDeviceContext;
HGLRC glRenderContext;

static bool isEngineActive = true;

WFSGL::wglSwapInterval_t* WFSGL::wglSwapInterval;

void WFSGL::UpdateViewport()
{
    float wasp = (float)nWindowWidth / (float)nWindowHeight;

    nViewW = (int32_t)nWindowWidth;
    nViewH = (int32_t)((float)nViewW / wasp);

    if (nViewH > nWindowHeight)
    {
        nViewH = nWindowHeight;
        nViewW = (int32_t)((float)nViewH * wasp);
    }

    nViewX = (nWindowWidth - nViewW) / 2;
    nViewY = (nWindowHeight - nViewH) / 2;
}


void WFSGL::WFSGLConstruct(int width, int height, LPCWSTR windowName) {
    nWindowWidth = width;
    nWindowHeight = height;
    halfScreenWidth = width / 2;
    halfScreenHeight = height / 2;
    name = windowName;
}

LRESULT CALLBACK WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:		isEngineActive = false;									return 0;
    case WM_DESTROY:	PostQuitMessage(0);										return 0;
    case WM_CREATE:		return 0;
    case WM_MOUSEMOVE:return 0;
    case WM_SIZE: return 0;
    case WM_MOUSEWHEEL: return 0;
    case WM_PAINT: return 0;
    case WM_MOUSELEAVE: 							return 0;
    case WM_SETFOCUS:									return 0;
    case WM_KILLFOCUS:								return 0;
    case WM_KEYDOWN:					return 0;
    case WM_KEYUP:					return 0;
    case WM_LBUTTONDOWN:							return 0;
    case WM_LBUTTONUP:							return 0;
    case WM_RBUTTONDOWN:						return 0;
    case WM_RBUTTONUP:						return 0;
    case WM_MBUTTONDOWN:					return 0;
    case WM_MBUTTONUP:						return 0;
    default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WFSGL::WFSGLCreateWindow() {
    
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowEvent;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandleA(0);
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = (LPCWSTR)"class001";
    wc.hIconSm = 0;

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        (LPCWSTR)"class001",
        (LPCWSTR)"DogENGINE",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE,
        30,
        30,
        nWindowWidth,
        nWindowHeight,
        0,
        0,
        GetModuleHandle(nullptr),
        0
    );

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    glDeviceContext = GetDC(hWnd);

    UpdateViewport();

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pf = 0;
    if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return false;
    SetPixelFormat(glDeviceContext, pf, &pfd);

    if (!(glRenderContext = wglCreateContext(glDeviceContext))) return false;
    wglMakeCurrent(glDeviceContext, glRenderContext);

    glViewport(nViewX, nViewY, nViewW, nViewH);

    wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapInterval) wglSwapInterval(0);

    return 0;
}


void WFSGL::WFSGLFill(Pixel p) {
    glClearColor((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern float deltaTime;

void WFSGL::WFSGLStartEngine() {
    glEnable(GL_TEXTURE_2D);

    on_start();

    glViewport(nViewX, nViewY, nViewW, nViewH);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0 && isEngineActive)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        auto time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration{ 0 };
        mainloop();

        SwapBuffers(glDeviceContext);

        auto endTime = std::chrono::high_resolution_clock::now();
        duration = endTime - time;
        deltaTime = duration.count();
        time = endTime;
        FPS = 1.0f / deltaTime;
        WFSGLChangeNameWithFPS(FPS);
    }

    wglDeleteContext(glRenderContext);
    PostMessage(hWnd, WM_DESTROY, 0, 0);
}

void WFSGL::WFSGLChangeNameWithFPS(float fps) {
    wchar_t s[256];
    swprintf_s(s, 256, L"%s - FPS: %3.2f", name, fps);
    SetWindowText(hWnd, s);
}
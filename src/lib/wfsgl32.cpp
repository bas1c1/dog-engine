#pragma once
#include "wfsgl32.h"

static bool isEngineActive = true;

void WFSGL::UpdateViewport()
{
    int32_t ww = width * windowScale;
    int32_t wh = height * windowScale;
    float wasp = (float)ww / (float)wh;

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

WFSGL::WFSGL() {
    this->width = 800;
    this->height = 600;
    this->nWindowWidth = width;
    this->nWindowHeight = height;
    this->name = L"Undefined";
    this->windowScale = 1;
    this->halfScreenWidth = width / 2;
    this->halfScreenHeight = height / 2;
    this->fSubPixelOffsetX = 0.0f;
    this->fSubPixelOffsetY = 0.0f;
    this->screenPixels = new Pixel[width * height];
    for (int i = 0; i < width * height; i++)
        this->screenPixels[i] = Pixel(0, 0, 0);
}

WFSGL::WFSGL(int width, int height, LPCWSTR name, int windowScale) {
    this->width = width;
    this->height = height;
    this->nWindowWidth = width;
    this->nWindowHeight = height;
    this->name = name;
    this->windowScale = windowScale;
    this->halfScreenWidth = width / 2;
    this->halfScreenHeight = height / 2;
    this->fSubPixelOffsetX = 0.0f;
    this->fSubPixelOffsetY = 0.0f;
    this->screenPixels = new Pixel[width * height];
    for (int i = 0; i < width * height; i++)
        this->screenPixels[i] = Pixel(0, 0, 0);
}

int WFSGL::WFSGLCreateWindow() {
    WNDCLASS wc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WindowEvent;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszMenuName = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = name;

    RegisterClass(&wc);

    nWindowWidth = (LONG)width * windowScale;
    nWindowHeight = (LONG)height * windowScale;

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE;

    int nCosmeticOffset = 30;
    nViewW = nWindowWidth;
    nViewH = nWindowHeight;

    UpdateViewport();

    GetDC(hWnd);

    RECT rWndRect = { 0, 0, nWindowWidth, nWindowHeight };
    AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
    int width = rWndRect.right - rWndRect.left;
    int height = rWndRect.bottom - rWndRect.top;
    hWnd = CreateWindowEx(dwExStyle, name, L"", dwStyle, nCosmeticOffset, nCosmeticOffset, width, height, NULL, NULL, GetModuleHandle(nullptr), nullptr);

    glDeviceContext = GetDC(hWnd);
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

LRESULT CALLBACK WFSGL::WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void WFSGL::WFSGLFillBuffer(std::vector<Pixel> pixels) {
    std::copy(pixels.begin(), pixels.end(), screenPixels);
}

void WFSGL::WFSGLFill(Pixel p) {
    glClearColor((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool WFSGL::WFSGLSetPixel(int x, int y, Pixel p) {
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        screenPixels[y * width + x] = p;
        return true;
    }
    else
        return false;
}

bool WFSGL::WFSGLDrawLine(int x1, int y1, int x2, int y2, Pixel p) {
    if ((x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
        && (x2 >= 0 && x2 < width && y2 >= 0 && y2 < height)
        )
    {

        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;
        WFSGLSetPixel(x2, y2, p);

        while (x1 != x2 || y1 != y2)
        {
            WFSGLSetPixel(x1, y1, p);
            int error2 = error * 2;
            if (error2 > -deltaY)
            {
                error -= deltaY;
                x1 += signX;
            }
            if (error2 < deltaX)
            {
                error += deltaX;
                y1 += signY;
            }
        }
        return true;
    }
    else
        return false;

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
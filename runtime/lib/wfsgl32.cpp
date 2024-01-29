#pragma once

#include "wfsgl32.h"
#include "vec.h"
#include "shaders.h"

const char* WFSGL::name;

LONG WFSGL::nWindowWidth;
LONG WFSGL::nWindowHeight;

LONG WFSGL::nViewW, WFSGL::nViewH, WFSGL::nViewX, WFSGL::nViewY;

GLfloat WFSGL::halfScreenWidth;
GLfloat WFSGL::halfScreenHeight;

GLuint WFSGL::glBuffer;

float WFSGL::FPS;

GLFWwindow* WFSGL::window;

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


void WFSGL::WFSGLConstruct(int width, int height, const char* windowName) {
    nWindowWidth = width;
    nWindowHeight = height;
    halfScreenWidth = width / 2;
    halfScreenHeight = height / 2;
    name = windowName;
}

int WFSGL::WFSGLCreateWindow() {

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    window = glfwCreateWindow(nWindowWidth, nWindowHeight, (const char*)name, NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    return 0;
}


void WFSGL::WFSGLFill(Pixel p) {
    glClearColor((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

extern float deltaTime;

void WFSGL::WFSGLStartEngine() {
    std::vector<Pixel> pixels = std::vector<Pixel>(800 * 200);

    const char* screen_vertex_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;"
        "layout(location = 1) in vec2 aTexCoords;"
        "out vec2 TexCoords;"
        "void main()"
        "{"
        "    TexCoords = aTexCoords;"
        "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);"
        "}";

    const char* screen_fragment_shader =
        "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec2 TexCoords;"
        "uniform sampler2D screenTexture;"
        "void main() {"
        "    vec3 col = texture(screenTexture, TexCoords).rgb;"
        "    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);"
        "}";

    const char* vertex_shader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;"
        "layout (location = 1) in vec3 color;"
        "layout (location = 2) in vec2 texCoord;"
        "out vec3 ourColor;"
        "out vec2 TexCoord;"
        "void main() {"
        "  gl_Position = vec4(position, 1.0f);"
        "  ourColor = color;"
        "  TexCoord = texCoord;"
        "}";

    const char* fragment_shader =
        "#version 330 core\n"
        "in vec3 ourColor;"
        "in vec2 TexCoord;"
        "out vec4 color;"
        "uniform sampler2D ourTexture;"
        "void main() {"
        "  color = texture(ourTexture, TexCoord);"
        "}";

    custom_shader main_shader(vertex_shader, fragment_shader);

    custom_shader screen_shader(screen_vertex_shader, screen_fragment_shader);

    UpdateViewport();

    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWindowWidth, nWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, nWindowWidth, nWindowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(nViewX, nViewY, nViewW, nViewH);
    on_start();

    while (!glfwWindowShouldClose(window)) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        main_shader.use();
        auto time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration{ 0 };
        mainloop();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screen_shader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        auto endTime = std::chrono::high_resolution_clock::now();
        duration = endTime - time;
        deltaTime = duration.count();
        time = endTime;
        FPS = 1.0f / deltaTime;
        WFSGLChangeNameWithFPS(FPS);
    }

    glfwTerminate();
}

void WFSGL::WFSGLChangeNameWithFPS(float fps) {
    char s[256];
    sprintf_s(s, 256, "%s - FPS: %3.2f", name, fps);

    glfwSetWindowTitle(window, s);
}
#include "shaders.h"
#include "../core/definitions.h"

custom_shader::custom_shader() {}

custom_shader::custom_shader(const char* vsc, const char* fsc) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsc, NULL);
    glCompileShader(vs);

    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsc, NULL);
    glCompileShader(fs);

    GLint isCompiled = 0;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fs, maxLength, &maxLength, &errorLog[0]);

        std::cout << errorLog.data();

        glDeleteShader(fs);
        return;
    }

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    this->prog = shader_programme;
}

void custom_shader::use() {
    glUseProgram(this->prog);
}

void custom_shader::set1f(const char* name, float a) {
    GLuint gScaleLocation = glGetUniformLocation(this->prog, name);
    glUniform1f(gScaleLocation, a);
}

void custom_shader::setVec2f(const char *name, vec2d a) {
    GLuint gScaleLocation = glGetUniformLocation(this->prog, name);
    glUniform2f(gScaleLocation, a.x, a.y);
}

void custom_shader::setVec3f(const char* name, vec3d a) {
    GLuint gScaleLocation = glGetUniformLocation(this->prog, name);
    glUniform3f(gScaleLocation, a.x, a.y, a.z);
}

void custom_shader::setVec4f(const char* name, vec4d a) {
    GLuint gScaleLocation = glGetUniformLocation(this->prog, name);
    glUniform4f(gScaleLocation, a.x, a.y, a.z, a.w);
}
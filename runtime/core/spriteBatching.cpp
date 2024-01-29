#include "definitions.h"

extern camera mainCamera;

spriteBatching::spriteBatching() {
	this->frames = std::vector<spriteFrame>();
	this->texture = 0;
    this->vaos = std::vector<GLfloat>();
}

GLuint spriteBatching::getTexture() {
	return this->texture;
}

void spriteBatching::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 32 * 10000, 0, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}

void spriteBatching::setTexture(GLuint texture) {
	this->texture = texture;
}

std::vector<spriteFrame> spriteBatching::getFrames() {
	return this->frames;
}

spriteFrame spriteBatching::getFrame(int index) {
	return this->frames[index];
}

void spriteBatching::setFrame(int index, spriteFrame frame) {
	this->frames[index] = frame;
}

void spriteBatching::pushFrame(spriteFrame frame) {
    this->frames.push_back(frame);
#define p(arg) this->vaos.push_back(arg);
    vec2d posF = mainCamera.project({ static_cast<float>(frame.x), static_cast<float>(frame.y) });

    std::vector<vec2d> s = std::vector<vec2d>{
        SCREEN_TO_NDC(vec2d((posF.x), (posF.y))),
        SCREEN_TO_NDC(vec2d((posF.x), (posF.y + frame.height))),
        SCREEN_TO_NDC(vec2d((posF.x + frame.width), (posF.y + frame.height))),
        SCREEN_TO_NDC(vec2d((posF.x + frame.width), (posF.y)))
    };

    p(s[3].x); p(s[3].y); p(0);   p(1.0f); p(0.0f); p(0.0f);   p(1.0f); p(1.0f);
    p(s[2].x); p(s[2].y); p(0);   p(0.0f); p(1.0f); p(0.0f);   p(1.0f); p(0.0f);
    p(s[1].x); p(s[1].y); p(0);   p(0.0f); p(0.0f); p(1.0f);   p(0.0f); p(0.0f);
    p(s[0].x); p(s[0].y); p(0);   p(1.0f); p(1.0f); p(0.0f);   p(0.0f); p(1.0f);

    s.clear();
#undef p(arg)
}

void spriteBatching::clearBuffers() {
    this->vaos.clear();
    this->frames.clear();
}

void spriteBatching::render() {
    std::vector<GLuint> indices(this->vaos.size() * 6);
    unsigned int j = 0;
    for (unsigned int i = 0; i < this->vaos.size() * 6; i += 6, j += 4) {
        indices[i] = j;
        indices[i + 1] = j + 1;
        indices[i + 2] = j + 2;
        indices[i + 3] = j + 2;
        indices[i + 4] = j + 3;
        indices[i + 5] = j;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vaos.size() * sizeof(GLfloat), this->vaos.data());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 2, indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    indices.clear();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDeleteBuffers(1, &EBO);
}
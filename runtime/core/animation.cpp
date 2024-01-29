#include "definitions.h"

extern float deltaTime;

animation::animation() {}

animation::animation(int delay) {
	this->delay = delay;
}

void animation::bindTransform(transform* tr) {
	this->tr = tr;
}

void animation::addSprite(sprite* sp) {
	this->sps.push_back(sp);
}

int animation::getDelay() {
	return this->delay;
}

void animation::setDelay(int delay) {
	this->delay = delay;
}

std::vector<sprite*> animation::getAllFrames() {
	return this->sps;
}

int index = 0;
int64_t globalFrameCounter = 0;

void animation::update() {
	if (this->delay == 0) {
		this->delay = 1;
	}

	if ((globalFrameCounter % this->delay) == 0) {
		if (index + 1 >= this->sps.size())
			index = 0;
		else
			index += 1;
	}
	sprite* currentSprite = this->sps[index];
	
    GLfloat vertices[] = {
        // Positions                        // Colors           // Texture Coords
         tr->drawable_square[3].x,  tr->drawable_square[3].y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         tr->drawable_square[2].x,  tr->drawable_square[2].y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
         tr->drawable_square[1].x,  tr->drawable_square[1].y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
         tr->drawable_square[0].x,  tr->drawable_square[0].y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
    };

    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, currentSprite->gpuTexture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

	globalFrameCounter += 1;
}

std::string animation::toString()
{
	return std::string("animation");
}
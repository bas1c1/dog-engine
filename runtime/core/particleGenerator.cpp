#include "definitions.h"
#include "../lib/io.h"

extern float deltaTime;

particleGenerator::particleGenerator() {

}

particleGenerator::particleGenerator(std::string filename, int amount, vec2d defaultSize, vec2d defaultVelocity, float defaultLifetime, vec2d pos) {
    this->texture = IO::generateTexture(IO::loadPNG(filename.c_str()));
    this->filename = filename;
    this->amount = amount;
    this->defaultSize = defaultSize;
    this->defaultVelocity = defaultVelocity;
    this->defaultLifetime = defaultLifetime;
    this->pos = pos;
    spb.init();
    for (int i = 0; i < amount; i++) {
        particleFrame pf;
        this->particles.push_back(pf);
    }
}

spriteFrame spf;

void particleGenerator::update() {
    this->spb.setTexture(this->texture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    int i = 0;
    for (particleFrame &particle : this->particles) {
        spf.height = defaultSize.y;
        spf.width = defaultSize.x;
        spf.x = particle.pos.x;
        spf.y = particle.pos.y;
        if (particle.life > 0.0f) {
            particle.life -= deltaTime;
            float random = ((rand() % 100) - 50) / 10.0f;
            float random2 = ((rand() % 100) - 50) / 10.0f;
            particle.pos.x += random * defaultVelocity.x * deltaTime * 10;
            particle.pos.y += random2 * defaultVelocity.y * deltaTime * 10;
            spf.x = particle.pos.x;
            spf.y = particle.pos.y;
        }
        else {
            particle.life = defaultLifetime / deltaTime;
            particle.pos = this->pos;
            
        }
        spb.pushFrame(spf);
        i += 1;
    }
    
    spb.render();
    spb.clearBuffers();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::string particleGenerator::toString() {
    return std::string("particle_generator");
}

std::string particleGenerator::getFilename() {
    return this->filename;
}

vec2d particleGenerator::getDefaultSize() {
    return this->defaultSize;
}

vec2d particleGenerator::getDefaultVelocity() {
    return this->defaultVelocity;
}

vec2d particleGenerator::getPos() {
    return this->pos;
}

float particleGenerator::getDefaultLifetime() {
    return this->defaultLifetime;
}

int particleGenerator::getAmount() {
    return this->amount;
}

void particleGenerator::setDefaultSize(vec2d defaultSize) {
    this->defaultSize = defaultSize;
    for (int i = 0; i < amount; i++) {
        this->particles[i].size = defaultSize;
    }
}

void particleGenerator::setDefaultVelocity(vec2d defaultVelocity) {
    this->defaultVelocity = defaultVelocity;
    for (int i = 0; i < amount; i++) {
        this->particles[i].velocity = defaultVelocity;
    }
}

void particleGenerator::setPos(vec2d pos) {
    this->pos = pos;
}

void particleGenerator::setDefaultLifetime(float defaultLifetime) {
    this->defaultLifetime = defaultLifetime;
}

void particleGenerator::setTexture(std::string texture) {
    this->filename = texture;
    this->texture = IO::generateTexture(IO::loadPNG(texture.c_str()));
}

void particleGenerator::setAmount(int amount) {
    this->amount = amount;
    this->particles.clear();
    for (int i = 0; i < amount; i++) {
        particleFrame pf;
        this->particles.push_back(pf);
    }
}
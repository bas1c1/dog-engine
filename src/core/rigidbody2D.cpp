#include "definitions.h"
extern float deltaTime;
extern scene mainScene;

int rigidbody2D::resolveCollision(rigidbody2D *other) {
    if (this->getType() == rigidbodyType::STATIC)
        return 0;

    int colDir = 0;

    transform* B = other->gameObj->get_component<transform>();
    transform* A = this->gameObj->get_component<transform>();

    collision2D col = this->gameObj->get_component<box_collider2D>()->getCollision();

    if (col.gameObject) {
        vec2d posA = A->getPos();

        int cd = 0;

        AABB boxA, boxB;

        boxA.min = A->square[0];
        boxA.max = A->square[2];

        boxB.min = B->square[0];
        boxB.max = B->square[2];

        float xPenetration = min(boxA.max.x - boxB.min.x, boxB.max.x - boxA.min.x);
        float yPenetration = min(boxA.max.y - boxB.min.y, boxB.max.y - boxA.min.y);

        if (xPenetration <= yPenetration) {
            if (boxA.min.x <= boxB.min.x) {
                posA.x -= xPenetration * mass;
                cd = 1;
            }
            else {
                posA.x += xPenetration * mass;
                cd = 1;
            }
        }
        else {
            if (boxA.min.y <= boxB.min.y) {
                posA.y -= yPenetration * mass;
                cd = 1;
            }
            else {
                posA.y += yPenetration * mass;
                cd = 1;
            }
        }

        if (cd != 0)
           this->impulse = 0;

        A->setPos(posA);
    }

    return colDir;
}

void rigidbody2D::bindTransform(transform* tr) {
	this->tr = tr;
}

void rigidbody2D::bindScene(scene* sc) {
    this->sc = sc;
}

void rigidbody2D::update() {
    if (rbType == rigidbodyType::DYNAMIC) {
        vec2d pos = tr->getPos();
        if (impulse <= maxImpulse)
            impulse += deltaTime;
        velocity.y += getGravity();
        pos = pos + velocity * impulse * mass;
        velocity.y -= getGravity();
        tr->setPos(pos);
    }
    for (object* obj : sc->getObjects()) {
        if (gameObj == obj) 
            continue;
        if (rbType == rigidbodyType::KINEMATIC)
            continue;
        rigidbody2D* other = obj->get_component<rigidbody2D>();
        resolveCollision(other);
    }
}

std::string rigidbody2D::toString() {
	return std::string("rigidbody2D");
}

void rigidbody2D::applyForce(vec2d force) {
	this->velocity = this->velocity + force;
}

void rigidbody2D::setVelocity(vec2d velocity) {
	this->velocity = velocity;
    this->impulse = 0;
}

vec2d rigidbody2D::getVelocity() {
	return this->velocity;
}

void rigidbody2D::setGravity(float gravity) {
	this->gravity = gravity;
    this->impulse = 0;
}

float rigidbody2D::getGravity() {
	return this->gravity;
}

void rigidbody2D::setMass(int mass) {
	this->mass = mass;
    this->impulse = 0;
}

int rigidbody2D::getMass() {
	return this->mass;
}

void rigidbody2D::setType(rigidbodyType rbType) {
    this->rbType = rbType;
}

rigidbodyType rigidbody2D::getType() {
    return this->rbType;
}
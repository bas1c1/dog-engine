#include "definitions.h"
#include "../lib/math.h"

void box_collider2D::bindTransform(transform* tr) {
    this->tr = tr;
}

void box_collider2D::bindScene(scene* sc) {
    this->sc = sc;
}

bool box_collider2D::detectCollision(transform* obj) {
    if (tr->square.size() <= 0) return false;
    if (obj->square.size() <= 0) return false;
    if (obj->square[2].x < tr->square[0].x or obj->square[0].x > tr->square[2].x) return false;
    if (obj->square[2].y < tr->square[0].y or obj->square[0].y > tr->square[2].y) return false;

    return true;
}

void box_collider2D::update() {

    for (object* obj : sc->getObjects()) {
        if (gameObj == obj)
            continue;

        collision2D col(obj, collisionType::BOX_COLLIDER_COLLISON2D);

        if (detectCollision(obj->get_component<transform>())) {
            if (std::find_if(collision.begin(), collision.end(), [col](const collision2D& p) { if (col.gameObject == p.gameObject) return true; return false; }) != collision.end()) { continue; }
            collision.push_back(col);
        }
        else {
            collision.erase(std::remove_if(collision.begin(),
                collision.end(),
                [col](const collision2D& p)
                {
                    return p.gameObject == col.gameObject;
                }),
                collision.end());
        }
    }
}

std::string box_collider2D::toString() {
    return std::string("box_collider_2d");
}

std::vector<collision2D> box_collider2D::getAllCollisions() {
    return collision;
}

collision2D box_collider2D::getCollision(int index) {
    if (collision.size() > index) {
        return collision[index];
    }
    else {
        return collision2D();
    }
}
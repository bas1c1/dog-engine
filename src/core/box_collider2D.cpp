#include "definitions.h"
#include "../lib/math.h"

void box_collider2D::bindTransform(transform* tr) {
    this->tr = tr;
    this->rect.fromTransform(tr);
}

void box_collider2D::bindScene(scene* sc) {
    this->sc = sc;
}

bool box_collider2D::detectCollision(transform* obj) {
    /*
    //OLD COLLISION DETECTION, USELESS

    this->rect.fromTransform(tr);
    if (!obj->gameObj->has_component<box_collider2D>())
        return false;
    rectangle a;
    a.fromTransform(obj);
    vec2d mainPos = rect.getPos();
    vec2d otherPos = a.getPos();

    const float pw = rect.getOffsetPos().x;
    const float ph = rect.getOffsetPos().y;

    return (mainPos.x + pw) > otherPos.x && (mainPos.x - pw) < otherPos.x && (mainPos.y + ph) > otherPos.y && (mainPos.y - ph) < otherPos.y;

    */

    /*

    

    if (obj->square[2].x < rect.square[0].x || rect.square[2].x < obj->square[0].x)
        return false;

    if (obj->square[2].y < rect.square[0].y || rect.square[2].y < obj->square[0].y)
        return false;*/

    this->rect.fromTransform(tr);

    if (rect.square.size() <= 0)
        return false;

    if (obj->square.size() <= 0)
        return false;
    
    AABB a, b;

    a.min = obj->square[0];
    a.max = obj->square[2];

    b.min = rect.square[0];
    b.max = rect.square[2];

    if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

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
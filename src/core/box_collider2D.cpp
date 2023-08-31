#include "definitions.h"
#include "../lib/math.h"

void box_collider2D::bindTransform(transform* tr) {
    this->tr = tr;
    this->rect.fromTransform(tr);
}

void box_collider2D::bindScene(scene* sc) {
    this->sc = sc;
}

bool box_collider2D::detectCollision(transform *obj) {
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

    this->rect.fromTransform(tr);

    for (int i = 0; i < 4; ++i) {
        int next = (i + 1) % 4;

        if (rect.square.size() <= 0)
            continue;
        
        vec2d p1 = rect.square[i];
        vec2d p2 = rect.square[next];

        for (int j = 0; j < 4; ++j) {
            int next2 = (j + 1) % 4;

            if (obj->square.size() <= 0)
                continue;
                
            vec2d q1 = obj->square[j];
            vec2d q2 = obj->square[next2];

            if ((p1.x == p2.x && p1.y == p2.y) || (q1.x == q2.x && q1.y == q2.y)) continue;

            if (isOverlap(p1.x, p2.x, q1.x, q2.x) && isOverlap(p1.y, p2.y, q1.y, q2.y)) 
            {
                float cross1 = (q2.x - q1.x) * (p1.y - q1.y) - (q2.y - q1.y) * (p1.x - q1.x);
                float cross2 = (p2.x - p1.x) * (p1.y - q1.y) - (p2.y - p1.y) * (p1.x - q1.x);
                float cross3 = (p2.x - p1.x) * (q1.y - p1.y) - (p2.y - p1.y) * (q1.x - p1.x);
                float cross4 = (q2.x - q1.x) * (q1.y - p1.y) - (q2.y - q1.y) * (q1.x - p1.x);

                if ((cross1 * cross2 > 0) || (cross3 * cross4 > 0)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void box_collider2D::update() {
    
    for (object *obj : sc->getObjects()) {
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
    if (collision.size() > index){
        return collision[index];
    }
    else {
        return collision2D();
    }
}
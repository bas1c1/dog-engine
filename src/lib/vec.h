#ifndef WSFGL_GEOMETRY
#define WSFGL_GEOMETRY

struct vec2d {
    float x;
    float y;

    vec2d() : x(0), y(0) {}

    vec2d(float x, float y) {
        this->x = x;
        this->y = y;
    }

    inline float getX() { return x; }
    inline float getY() { return y; }
    inline void setX(float x) { this->x = x; }
    inline void setY(float y) { this->y = y; }

    static vec2d zero() {
        return vec2d(0, 0);
    }
};

inline vec2d operator-(vec2d a, vec2d b) {
    vec2d out = a;
    out.x -= b.x;
    out.y -= b.y;
    return out;
}

inline vec2d operator+(vec2d a, vec2d b) {
    vec2d out = a;
    out.x += b.x;
    out.y += b.y;
    return out;
}

inline vec2d operator-(vec2d a, float b) {
    vec2d out = a;
    out.x -= b;
    out.y -= b;
    return out;
}

inline vec2d operator+(vec2d a, float b) {
    vec2d out = a;
    out.x += b;
    out.y += b;
    return out;
}

inline vec2d operator/(vec2d a, vec2d b) {
    vec2d out = a;
    out.x /= b.x;
    out.y /= b.y;
    return out;
}

inline vec2d operator*(vec2d a, vec2d b) {
    vec2d out = a;
    out.x *= b.x;
    out.y *= b.y;
    return out;
}

inline vec2d operator/(vec2d a, float b) {
    vec2d out = a;
    out.x /= b;
    out.y /= b;
    return out;
}

inline vec2d operator*(vec2d a, float b) {
    vec2d out = a;
    out.x *= b;
    out.y *= b;
    return out;
}

inline bool operator<(vec2d a, vec2d b) {
    return a.x < b.x&& a.y < b.y;
}

inline bool operator>(vec2d a, vec2d b) {
    return a.x > b.x && a.y > b.y;
}

struct vec3d {
    float x;
    float y;
    float z;

    vec3d() : x(0), y(0), z(0) {}

    vec3d(float x, float y, float z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3d(vec2d a) {
        this->x = a.x;
        this->y = a.y;
        this->z = 0;
    }

    inline float getX() { return x; }
    inline float getY() { return y; }
    inline float getZ() { return z; }
    inline void setX(float x) { this->x = x; }
    inline void setY(float y) { this->y = y; }
    inline void setZ(float z) { this->z = z; }

    static vec3d zero() {
        return vec3d(0, 0, 0);
    }
};

inline vec3d operator-(vec3d a, vec3d b) {
    vec3d out = a;
    out.x -= b.x;
    out.y -= b.y;
    out.z -= b.z;
    return out;
}

inline vec3d operator+(vec3d a, vec3d b) {
    vec3d out = a;
    out.x += b.x;
    out.y += b.y;
    out.z += b.z;
    return out;
}

inline vec3d operator-(vec3d a, float b) {
    vec3d out = a;
    out.x -= b;
    out.y -= b;
    out.z -= b;
    return out;
}

inline vec3d operator+(vec3d a, float b) {
    vec3d out = a;
    out.x += b;
    out.y += b;
    out.z += b;
    return out;
}

inline vec3d operator/(vec3d a, vec3d b) {
    vec3d out = a;
    out.x /= b.x;
    out.y /= b.y;
    out.z /= b.z;
    return out;
}

inline vec3d operator*(vec3d a, vec3d b) {
    vec3d out = a;
    out.x *= b.x;
    out.y *= b.y;
    out.z *= b.z;
    return out;
}

inline vec3d operator/(vec3d a, float b) {
    vec3d out = a;
    out.x /= b;
    out.y /= b;
    out.z /= b;
    return out;
}

inline vec3d operator*(vec3d a, float b) {
    vec3d out = a;
    out.x *= b;
    out.y *= b;
    out.z *= b;
    return out;
}

#endif
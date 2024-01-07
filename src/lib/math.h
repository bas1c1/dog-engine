#ifndef DOG_ENGINE_MATH
#define DOG_ENGINE_MATH
#include <minwindef.h>
#include "vec.h"

#define ToRadian(x) ((x) * 3.141593f / 180.0f)

static bool isOverlap(double a, double b, double c, double d) {
    return max(a, c) <= min(b, d);
}

static float clamp(float value, float min, float max) { return fmax(fmin(value, max), min); }
static double sign(double a) { return (0 < a) - (a < 0); }
static double step(double edge, double x) { return x > edge; }
static float lerp(float a, float b, float t) { return a + t * (b - a); }
static float dist(const vec2d& p1, const vec2d& p2) {
    float diffY = p1.y - p2.y;
    float diffX = p1.x - p2.x;
    return sqrt((diffY * diffY) + (diffX * diffX));
}
static double dot(const vec2d& a, const vec2d& b) { return (a.x * b.x) + (a.y * b.y); }
static double perpDot(const vec2d& a, const vec2d& b) { return (a.y * b.x) - (a.x * b.y); }

#endif
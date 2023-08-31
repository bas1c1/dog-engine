#ifndef DOG_ENGINE_MATH
#define DOG_ENGINE_MATH
#include <minwindef.h>

#define ToRadian(x) ((x) * 3.141593f / 180.0f)

static bool isOverlap(double a, double b, double c, double d) {
    return max(a, c) <= min(b, d);
}


#endif
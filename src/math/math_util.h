#include "graph.h"

#ifndef MATH_UTIL_H
#define MATH_UTIL_H

f32 frsqrt(f32 number);
f32 to_radians(f32 angle);
f32 to_degrees(f32 angle);

#define sqr(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))
#define PI 3.14159265358979323846264338327
#define MIN(x,y) ((x < y) ? x : y)
#define MAX(x,y) ((x > y) ? x : y)
#define CLAMP(x,y,z) (MIN(MAX(x,y),z))

#endif
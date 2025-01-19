#include "../types.h"
#include "math_util.h"

/**
 * @internal
 * @brief Fast inverse square root.
 */

f32 frsqrt(f32 number) {
  union {
    f32 f;
    u32 i;
  } conv = { .f = number };
  conv.i  = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
  return conv.f;
}

/**
 * @internal
 * @brief Convert degrees to radians.
 */

f32 to_radians(f32 angle) {
  return angle*(PI/180.0f);
}


/**
 * @internal
 * @brief Convert radians to degrees.
 */

f32 to_degrees(f32 angle) {
  return angle*(180.0f/PI);
}
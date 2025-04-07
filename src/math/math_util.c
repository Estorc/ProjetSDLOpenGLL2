#include <raptiquax.h>
#include <math/math_util.h>

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


f32 float_approach(f32 * const origin, const f32 dest, const f32 factor) {
  *origin += (dest - *origin) * factor;
  return *origin;
}

float * vec3_approach(float * origin, float * dest, const f32 factor) {
  origin[0] += (dest[0] - origin[0]) / factor;
  origin[1] += (dest[1] - origin[1]) / factor;
  origin[2] += (dest[2] - origin[2]) / factor;
  return origin;
}

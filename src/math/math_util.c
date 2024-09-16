#include "../types.h"
#include "math_util.h"

/**
 * Computes the fast inverse square root of a given number using the 
 * "Fast Inverse Square Root" algorithm.
 * 
 * @param number {f32} The input number for which the inverse square root 
 *                     is to be calculated. Must be a positive number.
 * @return {f32} The approximate value of the inverse square root of the 
 *               input number.
 * 
 * This function utilizes a clever bit manipulation technique to compute 
 * the fast inverse square root, originally popularized by its use in 
 * the Quake III Arena graphics engine. The algorithm performs an 
 * initial approximation using a magic constant and then refines the 
 * result using one iteration of Newton's method for improved accuracy.
 * 
 * The formula used for the approximation is as follows:
 * 
 *     x = number
 *     i = *(int*)&x        // Bit-level hacking to get integer representation
 *     i = 0x5f3759df - (i >> 1) // The magic constant
 *     x = *(float*)&i      // Convert back to float
 *     x = x * (1.5 - (number * 0.5 * x * x)) // Newton's method refinement
 * 
 * This function is typically used in graphics and game development, 
 * where performance is critical, and is suitable for scenarios where 
 * the input number is non-negative.
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
 * Converts an angle from degrees to radians.
 * 
 * @param angle {f32} The angle in degrees to be converted.
 * @return {f32} The converted angle in radians.
 * 
 * This function takes an angle specified in degrees and converts it to 
 * radians using the formula:
 * 
 *     radians = degrees * (PI / 180)
 * 
 * where PI is a constant representing the mathematical constant π. 
 * The function returns the equivalent angle in radians, allowing for 
 * easier interpretation and usage in applications where radians are preferred.
 */

f32 to_radians(f32 angle) {
  return angle*(PI/180);
}


/**
 * Converts an angle from radians to degrees.
 * 
 * @param angle {f32} The angle in radians to be converted.
 * @return {f32} The converted angle in degrees.
 * 
 * This function takes an angle specified in radians and converts it to 
 * degrees using the formula:
 * 
 *     degrees = radians * (180 / PI)
 * 
 * where PI is a constant representing the mathematical constant π. 
 * The function returns the equivalent angle in degrees, allowing for 
 * easier interpretation and usage in applications where degrees are preferred.
 */

f32 to_degrees(f32 angle) {
  return angle*(180/PI);
}

/*f32 sqrt(f32 number) {
    return 1/frsqrt(number);
}*/
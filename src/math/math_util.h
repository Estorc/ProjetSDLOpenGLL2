#ifndef MATH_UTIL_H
#define MATH_UTIL_H

/**
 * @defgroup MathGroup Math Utilities
 * @brief Bunch of math functions.
 * @{
 */

/**
 * @brief Computes the fast inverse square root of a given number using the 
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

f32 frsqrt(f32 number);

/**
 * @brief Converts an angle from degrees to radians.
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

f32 to_radians(f32 angle);

/**
 * @brief Converts an angle from radians to degrees.
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

f32 to_degrees(f32 angle);

/**
 * @brief Computes the square of a number.
 * 
 * This macro takes a number and returns its square by multiplying the 
 * number by itself.
 * 
 * @param x The number to be squared.
 * @return The square of the input number.
 */
#define sqr(x) ((x)*(x))

/**
 * @brief Computes the cube of a number.
 * 
 * This macro takes a number and returns its cube by multiplying the 
 * number by itself twice.
 * 
 * @param x The number to be cubed.
 * @return The cube of the input number.
 */
#define cube(x) ((x)*(x)*(x))

/**
 * @brief The mathematical constant π (pi).
 * 
 * This macro defines the value of π (pi) to a high degree of precision.
 */
#define PI 3.14159265358979323846264338327

/**
 * @brief Computes the minimum of two values.
 * 
 * This macro takes two values and returns the smaller of the two.
 * 
 * @param x The first value.
 * @param y The second value.
 * @return The smaller of the two input values.
 */
#define MIN(x,y) ((x < y) ? x : y)

/**
 * @brief Computes the maximum of two values.
 * 
 * This macro takes two values and returns the larger of the two.
 * 
 * @param x The first value.
 * @param y The second value.
 * @return The larger of the two input values.
 */
#define MAX(x,y) ((x > y) ? x : y)

/**
 * @brief Clamps a value between a minimum and maximum range.
 * 
 * This macro takes a value and ensures it falls within the specified 
 * range. If the value is less than the minimum, it returns the minimum. 
 * If the value is greater than the maximum, it returns the maximum. 
 * Otherwise, it returns the value itself.
 * 
 * @param x The value to be clamped.
 * @param y The minimum allowable value.
 * @param z The maximum allowable value.
 * @return The clamped value.
 */
#define CLAMP(x,y,z) (MIN(MAX(x,y),z))

/**
 * @brief Approaches a target vector by a given factor.
 * 
 * @param a {vec3} The vector to be approached.
 * @param b {vec3} The target vector.
 * @param t {f32} The approach factor.
 * 
 * This macro moves vector a towards vector b by a factor of t.
 */
#define Vec3fApproach(a,b,t) ((a)[0]+=((b)[0]-(a)[0])/t,(a)[1]+=((b)[1]-(a)[1])/t,(a)[2]+=((b)[2]-(a)[2])/t)

/** @} */

#endif
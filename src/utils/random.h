#pragma once

/**
 * @file random.h
 * @brief Utility functions for generating random numbers.
 *
 * This header file provides functions to initialize the random number generator
 * and to generate random floating-point and integer numbers within specified ranges.
 */

/**
 * @brief Initializes the random number generator.
 *
 * This function should be called before any other random number functions
 * to ensure that the random numbers generated are properly seeded.
 */
void init_random();

/**
 * @brief Generates a random floating-point number within a specified range.
 *
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return A random floating-point number between min and max.
 */
float random_float(float min, float max);

/**
 * @brief Generates a random integer within a specified range.
 *
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return A random integer between min and max.
 */
int random_int(int min, int max);
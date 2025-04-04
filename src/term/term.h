#pragma once
#include <term/enhanced_print.h>

/**
 * @brief Prints a 3D vector with a given name.
 *
 * This function takes a pointer to a 3D vector (array of 3 floats) and a name,
 * and prints the vector in a formatted manner.
 *
 * @param v Pointer to the first element of the 3D vector (array of 3 floats).
 * @param name The name to be printed alongside the vector.
 */
void fast_vec3_print(const float *v, const char *name);
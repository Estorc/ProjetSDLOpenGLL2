#include "../types.h"
#include "math_util.h" 
#include "../io/model.h"


/**
 * Computes the magnitude (length) of a 3-dimensional vector.
 * 
 * @param vec {Vec3f*} Pointer to a Vec3f structure representing the vector.
 * @return {f32} The magnitude of the vector as a floating-point value.
 * 
 * This function calculates the magnitude of the vector by taking the square root of the sum of 
 * the squares of its components. The magnitude is defined as the length of the vector in 3D space.
 * 
 * The formula used is:
 * 
 *     |v| = √(x² + y² + z²)
 * 
 * where x, y, and z are the components of the vector.
 */

f32 get_vec3f_magnitude(Vec3f *vec) {
    return sqrt(sqr(*(vec)[0]) + sqr(*(vec)[1]) + sqr(*(vec)[2]));
}

/**
 * Computes the squared magnitude (length squared) of a 3-dimensional vector.
 * 
 * @param vec {Vec3f*} Pointer to a Vec3f structure representing the vector.
 * @return {f32} The squared magnitude of the vector as a floating-point value.
 * 
 * This function calculates the squared magnitude of the vector without taking the 
 * square root. The squared magnitude is defined as the sum of the squares of the 
 * vector's components.
 * 
 * The formula used is:
 * 
 *     |v|² = x² + y² + z²
 * 
 * where x, y, and z are the components of the vector.
 * 
 * This function is useful for comparisons of vector lengths, as it avoids the 
 * computational cost of the square root operation.
 */

f32 get_vec3f_sqr_magnitude(Vec3f *vec) {
    return sqr(*(vec)[0]) + sqr(*(vec)[1]) + sqr(*(vec)[2]);
}

/**
 * Sets the magnitude of a 3-dimensional vector to a specified value.
 * 
 * @param vec {Vec3f*} Pointer to a Vec3f structure representing the vector.
 * @param magnitude {f32} The desired magnitude for the vector.
 * 
 * This function adjusts the vector's length to the specified magnitude while maintaining 
 * its direction. It first computes the current magnitude of the vector, then scales 
 * the vector accordingly.
 * 
 * The scaling is achieved by dividing the desired magnitude by the current magnitude 
 * and multiplying each component of the vector by this scale factor. 
 * 
 * If the current magnitude is zero, the vector will remain unchanged as scaling 
 * would not be defined in this case.
 */

void set_vec3f_magnitude(Vec3f *vec, f32 magnitude) {
    magnitude /= get_vec3f_magnitude(vec);
    Vec3fScale((*vec),magnitude);
}

/**
 * Specifies a polygon by passing a set of 3D vertices to OpenGL.
 * 
 * @param vecs {Vec3f*} Pointer to an array of Vec3f structures representing the vertices of the polygon.
 * @param length {u8} The number of vertices in the polygon.
 * 
 * This function iterates through the provided array of 3D vertices and calls 
 * the `glVertexVec3` function for each vertex, effectively defining the shape 
 * of the polygon in the OpenGL context. The order of the vertices determines 
 * how the polygon is rendered.
 * 
 * It is assumed that the OpenGL context has been properly set up and that the 
 * appropriate polygon mode (e.g., GL_FILL, GL_LINE) has been set prior to 
 * calling this function. The function does not perform any error checking 
 * regarding the validity of the vertices or OpenGL state.
 */

void glPolygonVec3(Vec3f *vecs, u8 length) {
    for (int i = 0; i<length; i++) glVertexVec3( vecs[i] );
}
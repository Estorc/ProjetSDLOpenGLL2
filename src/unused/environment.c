#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "types.h"
#include "color.h"
#include "math_util.h"


/**
 * Configures a RGBA light in the OpenGL context at a specified position with ambient and diffuse colors.
 * 
 * @param lightId {u32} The identifier of the light source to configure. This should correspond 
 *                      to one of OpenGL's light source IDs (e.g., GL_LIGHT0, GL_LIGHT1, etc.).
 * @param pos {Vec3f*} Pointer to a Vec3f structure representing the position of the light in world space.
 * @param ambientCol {ColRGBA} The color of the ambient light component, defined as a ColRGBA structure.
 * @param diffuseCol {ColRGBA} The color of the diffuse light component, defined as a ColRGBA structure.
 * 
 * This function sets up the specified light source in OpenGL with the provided ambient and diffuse 
 * color values. The light's position is also specified in homogeneous coordinates (with w set to 1.0f).
 * 
 * Note: This function does not enable the light; to enable it, the corresponding call to glEnable(lightId) 
 * should be made elsewhere in the code.
 */

void place_rgba_light(u32 lightId, Vec3f *pos, ColRGBA ambientCol, ColRGBA diffuseCol) {

    float lightDiffuse[] = {diffuseCol.r, diffuseCol.g, diffuseCol.b, diffuseCol.a};
    float lightAmbient[] = {ambientCol.r, ambientCol.g, ambientCol.b, ambientCol.a};
    float lightPosition[] = {*(pos)[0], *(pos)[1], *(pos)[2], 1.0f};

    //Ambient light component
    glLightfv(lightId, GL_AMBIENT, lightAmbient);
    //Diffuse light component
    glLightfv(lightId, GL_DIFFUSE, lightDiffuse);
    //Light position
    glLightfv(lightId, GL_POSITION, lightPosition);
    //glEnable(lightId);
}
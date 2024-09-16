#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>

/**
 * Initializes the OpenGL viewport settings.
 * 
 * This function enables several OpenGL features that are essential for rendering 
 * graphics correctly in a 2D and 3D environment. Specifically, it enables 
 * texture mapping, sRGB framebuffer support for accurate color management, 
 * multisampling for anti-aliasing, and depth testing to ensure correct 
 * rendering order based on depth values.
 * 
 * Features enabled:
 * - GL_TEXTURE_2D: Enables 2D texturing.
 * - GL_FRAMEBUFFER_SRGB: Enables sRGB framebuffer for gamma-correct rendering.
 * - GL_MULTISAMPLE: Enables multi-sampling anti-aliasing.
 * - GL_DEPTH_TEST: Enables depth testing to ensure proper rendering of 
 *   overlapping objects based on their distance from the camera.
 */

void init_viewport() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FRAMEBUFFER_SRGB); 
    glEnable(GL_MULTISAMPLE);  
    glEnable(GL_DEPTH_TEST);
}
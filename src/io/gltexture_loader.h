#ifndef GLTEXTURE_LOADER_H
#define GLTEXTURE_LOADER_H

/**
 * @defgroup TextureLoader Texture Loader
 * @brief Functions for loading and manipulating textures.
 * 
 * This module provides functions for loading textures from files, converting 
 * them to different formats, and manipulating their properties for use in 
 * OpenGL rendering.
 * @{
 */

/**
 * @brief Converts an SDL_Surface with RGBA format to an array of floats representing an RGB32F texture.
 *
 * @param rgbaSurface {SDL_Surface*} A pointer to the SDL_Surface containing the RGBA texture data.
 * @return {float*} A pointer to the array of floats representing the RGB32F texture data.
 *
 * This function takes an SDL_Surface that contains RGBA texture data and converts 
 * it to an array of floats representing an RGB32F texture. This format is suitable 
 * for high dynamic range (HDR) rendering in OpenGL.
 */

float * convert_to_rgb32f_texture(SDL_Surface *rgbaSurface);

/**
 * @brief Flips an SDL_Surface vertically (along the Y-axis).
 *
 * @param surface {SDL_Surface*} A pointer to the SDL_Surface to be flipped.
 * @return {SDL_Surface*} A pointer to the new SDL_Surface that has been flipped vertically.
 *
 * This function creates a new SDL_Surface that is a vertically flipped version 
 * of the input surface. This is useful for correcting the orientation of textures 
 * that are loaded upside down.
 */

SDL_Surface * flip_y_surface(SDL_Surface *surface);

/**
 * @brief Loads a texture from a specified file path and creates an OpenGL texture.
 *
 * @param path {char*} A string containing the file path to the texture image.
 * @param format {GLenum} The format of the texture (e.g., GL_RGBA, GL_RGB).
 * @param yReversed {bool} A boolean indicating whether the texture's Y-axis should be reversed.
 * @return {TextureMap} The generated OpenGL texture ID, or 0 if loading fails.
 *
 * This function utilizes SDL_image to load a texture from the specified file 
 * path, creates an OpenGL texture object, and configures its parameters. The 
 * loaded texture is suitable for use in rendering graphics with OpenGL.
 *
 * Example Usage:
 * @code
 * TextureMap texture = load_texture_from_path("path/to/texture.png", GL_RGBA, true);
 * if (texture != 0) {
 *     // Use the texture for rendering...
 * }
 * @endcode
 *
 * @return Returns the OpenGL texture ID on success. If the texture fails to load, 
 * the function returns 0.
 */

TextureMap load_texture_from_path(char * path, GLenum format, bool yReversed);

/**
 * @brief Renders text onto an SDL_Surface at a specified position.
 *
 * @param render_surface {SDL_Surface*} A pointer to the SDL_Surface where the text will be rendered.
 * @param x {int} The x-coordinate where the text will be rendered.
 * @param y {int} The y-coordinate where the text will be rendered.
 * @param text {char*} A string containing the text to be rendered.
 * @param font {TTF_Font*} A pointer to the TTF_Font to be used for rendering the text.
 * @param color {SDL_Color} The color of the text.
 * @param alignment {char*} A string specifying the alignment of the text ("left", "center", "right").
 * @param width {int} The width of the area in which the text will be rendered.
 *
 * This function renders the specified text onto the given SDL_Surface at the 
 * specified position. The text is rendered using the provided TTF_Font and color. 
 * The alignment parameter specifies how the text should be aligned within the 
 * given width.
 */

void draw_text(SDL_Surface *render_surface, int x, int y, char *text, TTF_Font *font, SDL_Color color, char *alignment, int width);

/** @} */

#endif
#ifndef SKYBOX_H
#define SKYBOX_H

/**
 * @defgroup SkyboxUtilities Skybox Utilities
 * @brief Utility functions for creating and managing skyboxes.
 * @{
 */

/**
 * @brief Loads a cubemap texture from 6 individual texture faces.
 * 
 * This function takes an array of file paths to 6 texture images, each representing
 * one face of a cubemap (right, left, top, bottom, front, back), and loads them into
 * a single cubemap texture.
 * 
 * @param faces An array of 6 strings, each containing the file path to a texture image.
 *              The order of the faces should be: right, left, top, bottom, front, back.
 * 
 * @return TextureMap The loaded cubemap texture.
 */
TextureMap load_cubemap(char faces[6][200]);

/**
 * @brief Creates a skybox with the given textures.
 *
 * This function initializes a skybox using the provided textures and assigns it to the given textured mesh.
 *
 * @param texturedMesh A pointer to the TextureMap structure where the skybox will be created.
 * @param skyboxTexture A 2D array containing the file paths of the six textures for the skybox. 
 *                      The textures should be ordered as follows: right, left, top, bottom, front, back.
 * @note Each file path in the skyboxTexture array should not exceed 200 characters.
 */

void create_skybox(TextureMap *texturedMesh, char skyboxTexture[6][200]);

/** @} */

#endif
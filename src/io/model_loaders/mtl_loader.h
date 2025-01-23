#pragma once

/**
 * @defgroup MTLLoader MTL Loader
 * @brief Functions to load and manage materials from .mtl files.
 * @{
 */

/**
 * @brief Loads material properties from a .mtl file and stores them in an array.
 *
 * @param path {char*} The file path where the .mtl file is located.
 * @param filename {char*} The name of the .mtl file to load.
 * @param materials {Material**} A pointer to an array of Material structures 
 *                               that will be populated with the loaded materials.
 *
 * This function reads material properties from the specified .mtl file 
 * and allocates memory for an array of Material structures. The materials 
 * are populated with properties such as color, texture maps, and other 
 * settings as defined in the .mtl file format.
 *
 * The following properties are read:
 * - Name of the material
 * - Ambient, diffuse, and specular colors
 * - Specular exponent
 * - Optical density
 * - Illumination flag
 * - Opacity
 * - Texture maps (diffuse, normal, displacement)
 *
 * If a material with the same name already exists, it will be skipped.
 * 
 * The function dynamically allocates memory for the materials and resizes 
 * the allocation if necessary, doubling the size of the materials array 
 * when more space is needed.
 *
 * It utilizes OpenGL to generate and bind textures for the material's 
 * texture maps using SDL for image loading.
 *
 * The function returns the number of materials loaded or -1 if an error 
 * occurs.
 *
 * Example Usage:
 * Material *materials;
 * int materialCount = load_mtl("path/to/materials/", "material.mtl", &materials);
 * if (materialCount == -1) {
 *     // Handle error...
 * } else {
 *     // Process loaded materials...
 * }
 *
 * @return Returns the number of materials loaded or -1 on error.
 */

int load_mtl(char *path, char *filename, Material **materials);

/**
 * @brief Searches for a material by its name in a list of materials.
 *
 * @param materials {Material*} A pointer to an array of Material structures.
 * @param materialsCount {int} The total number of materials in the array.
 * @param materialName {char*} The name of the material to search for.
 *
 * This function iterates through the array of Material structures to 
 * find a material that matches the specified name. It performs a 
 * string comparison using `strcmp` to check for equality.
 *
 * If a matching material is found, the function returns the index of 
 * that material in the array. If no match is found, it returns -1.
 *
 * Example Usage:
 * int index = find_material(materials, materialsCount, "Wood");
 * if (index != -1) {
 *     // Material found, process it...
 * } else {
 *     // Material not found...
 * }
 *
 * @return Returns the index of the found material or -1 if not found.
 */

int find_material(Material *materials, int materialsCount, char *materialName);

/** @} */
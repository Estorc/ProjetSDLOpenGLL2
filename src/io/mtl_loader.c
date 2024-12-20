#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "../math/math_util.h"
#include "model.h"


/**
 * Loads material properties from a .mtl file and stores them in an array.
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
 * Return:
 * Returns the number of materials loaded or -1 on error.
 */


int load_mtl(char *path, char *filename, Material **materials) {
    
    char *fullPath = malloc(strlen(path) + 1 + strlen(filename) + 1);
    POINTER_CHECK(fullPath);
    FILE * file = fopen(strcat(strcpy(fullPath, path), filename), "r");
    POINTER_CHECK(file);
    free(fullPath);

    u8 mi = -1, mim = 2;
    
    *materials = malloc(sizeof(Material) * mim);
    POINTER_CHECK(*materials);

    if (!*materials) return -1;
    
    char symbol;
    do {
        if (mi+1 >= mim) {
            mim<<=1;
            
            *materials = realloc(*materials, sizeof(Material) * mim);
            POINTER_CHECK(*materials);
            
        }
        if (!*materials) return -1;
        symbol = getc(file);
        switch (symbol) {
        case 'n': ;
            fscanf(file, "%*[^ ] %s", (char *) &(*materials)[++mi].name);
            (*materials)[mi].opacity = 1.0f;
            (*materials)[mi].opticalDensity = 1.0f;
            (*materials)[mi].specularExp = 64.0f;
            for (int i = 0; i < MATERIAL_PROPERTY_COUNT; i++) {
                glm_vec3_copy((vec3) {0.5f, 0.5f, 0.5f}, (*materials)[mi].flatColors[i]);
                (*materials)[mi].textureMaps[i] = 0;
            }
            break;
        case 'N': ;
            symbol = getc(file);
            switch (symbol) {
                case 's': ;
                    fscanf(file, "%f\n", &(*materials)[mi].specularExp);
                    break;
                case 'i': ;
                    fscanf(file, "%f\n", &(*materials)[mi].opticalDensity);
                    break;
                default: fscanf(file, "%*[^\n]\n");
            }
            break;
        case 'K': ;
            symbol = getc(file);
            switch (symbol) {
                case 'a': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[AMBIENT_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[AMBIENT_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[AMBIENT_MATERIAL_PROPERTY][2]);
                    break;
                case 's': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[SPECULAR_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[SPECULAR_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[SPECULAR_MATERIAL_PROPERTY][2]);
                    break;
                case 'd': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[DIFFUSE_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[DIFFUSE_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[DIFFUSE_MATERIAL_PROPERTY][2]);
                    break;
                default: fscanf(file, "%*[^\n]\n");
            }
            break;
        case 'P': ;
            symbol = getc(file);
            switch (symbol) {
                case 'r': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[ROUGHNESS_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[ROUGHNESS_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[ROUGHNESS_MATERIAL_PROPERTY][2]);
                    break;
                case 'm': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[METALLIC_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[METALLIC_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[METALLIC_MATERIAL_PROPERTY][2]);
                    break;
                case 'x': ;
                    fscanf(file, "%f\n", 
                        &(*materials)[mi].flatColors[PARALLAX_MATERIAL_PROPERTY][0]);
                    break;
                default: fscanf(file, "%*[^\n]\n");
            }
            break;
        case 'i': ;
            fscanf(file, "%hhd\n", &(*materials)[mi].illumFlag);
            break;
        case 'd': ;
            fscanf(file, "%f\n", &(*materials)[mi].opacity);
            break;
        case 'm': ;
            
            char textureFilename[50];
            char textureType[20];
            fscanf(file, "%*[^_]_%s %s\n", (char *) &textureType, (char *) &textureFilename);
            char *fullPath = malloc(strlen(path) + 1 + strlen(textureFilename) + 1);
            POINTER_CHECK(fullPath);
            strcat(strcpy(fullPath, path), textureFilename);
            if (!strcmp(textureType, "Px"))
                (*materials)[mi].textureMaps[PARALLAX_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_RGB32F, true);
            if (!strcmp(textureType, "Bump"))
                (*materials)[mi].textureMaps[NORMAL_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_RGB32F, true);
            if (!strcmp(textureType, "Kd"))
                (*materials)[mi].textureMaps[DIFFUSE_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_SRGB_ALPHA, true);
            if (!strcmp(textureType, "Pr"))
                (*materials)[mi].textureMaps[ROUGHNESS_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_RGB32F, true);
            if (!strcmp(textureType, "Pm"))
                (*materials)[mi].textureMaps[METALLIC_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_RGB32F, true);
            free(fullPath);

            break;
        case '#': ;
        default:
            fscanf(file, "%*[^\n]\n");
            break;
        }
    } while (symbol != -1);

    
    *materials = realloc(*materials, sizeof(Material) * (mi+1));
    POINTER_CHECK(*materials);
    

    return mi+1;

}


/**
 * Searches for a material by its name in a list of materials.
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
 * Return:
 * Returns the index of the found material or -1 if not found.
 */

int find_material(Material *materials, int materialsCount, char *materialName) {
    for (int i = 0; i < materialsCount; i++) {
        if (!strcmp(materials[i].name, materialName)) return i;
    }
    return -1;
}
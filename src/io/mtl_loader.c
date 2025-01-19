#include "../types.h"
#include "../math/math_util.h"
#include "model.h"


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
            fscanf(file, "%*[^ ] %s\n", (char *) &(*materials)[++mi].name);
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
                case 'e': ;
                    fscanf(file, "%f %f %f\n", 
                        &(*materials)[mi].flatColors[EMISSION_MATERIAL_PROPERTY][0], 
                        &(*materials)[mi].flatColors[EMISSION_MATERIAL_PROPERTY][1], 
                        &(*materials)[mi].flatColors[EMISSION_MATERIAL_PROPERTY][2]);
                    break;
                default: fscanf(file, "%*[^\n]\n");
            }
            break;
        case 'P': ;
            symbol = getc(file);
            switch (symbol) {
                case 'r': ;
                    fscanf(file, "%f\n", 
                        &(*materials)[mi].flatColors[ROUGHNESS_MATERIAL_PROPERTY][0]);
                    break;
                case 'm': ;
                    fscanf(file, "%f\n", 
                        &(*materials)[mi].flatColors[METALLIC_MATERIAL_PROPERTY][0]);
                    break;
                case 's': ;
                    fscanf(file, "%f\n", 
                        &(*materials)[mi].flatColors[SHEEN_MATERIAL_PROPERTY][0]);
                    break;
                case 'x': ;
                    fscanf(file, "%f\n", 
                        &(*materials)[mi].flatColors[PARALLAX_MATERIAL_PROPERTY][0]);
                    break;
                case 'c': ;
                    symbol = getc(file);
                    switch (symbol) {
                        case ' ': ;
                            fscanf(file, "%f\n", 
                                &(*materials)[mi].flatColors[CLEARCOAT_THICKNESS_MATERIAL_PROPERTY][0]);
                            break;
                        case 'r': ;
                            fscanf(file, "%f\n", 
                                &(*materials)[mi].flatColors[CLEARCOAT_ROUGHNESS_MATERIAL_PROPERTY][0]);
                            break;
                        default: fscanf(file, "%*[^\n]\n");
                    }
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
            fscanf(file, "%*[^_]_%s %[^\n]\n", (char *) &textureType, (char *) &textureFilename);
            char *fullPath = malloc(strlen(path) + 1 + strlen(textureFilename) + 1);
            POINTER_CHECK(fullPath);
            strcat(strcpy(fullPath, path), textureFilename);
            if (!strcmp(textureType, "Kd"))
                (*materials)[mi].textureMaps[DIFFUSE_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_SRGB8_ALPHA8, true);
            if (!strcmp(textureType, "Ks"))
                (*materials)[mi].textureMaps[SPECULAR_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_SRGB8, true);
            if (!strcmp(textureType, "Ke"))
                (*materials)[mi].textureMaps[EMISSION_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_SRGB8, true);
            if (!strcmp(textureType, "Px"))
                (*materials)[mi].textureMaps[PARALLAX_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_R16, true);
            if (!strcmp(textureType, "Pr"))
                (*materials)[mi].textureMaps[ROUGHNESS_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_R16, true);
            if (!strcmp(textureType, "Pm"))
                (*materials)[mi].textureMaps[METALLIC_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_R16, true);
            if (!strcmp(textureType, "Ps"))
                (*materials)[mi].textureMaps[SHEEN_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_R16, true);
            if (!strcmp(textureType, "Bump"))
                (*materials)[mi].textureMaps[NORMAL_MATERIAL_PROPERTY] = load_texture_from_path(fullPath, GL_RGB8, true);
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


int find_material(Material *materials, int materialsCount, char *materialName) {
    for (int i = 0; i < materialsCount; i++) {
        if (!strcmp(materials[i].name, materialName)) return i;
    }
    return -1;
}
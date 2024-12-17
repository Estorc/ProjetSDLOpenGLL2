#include <stdio.h>
#include "types.h"
#include "math/math_util.h"
#include "io/model.h"
#include "io/shader.h"
#include "memory.h"

void init_memory_cache() {
    memoryCaches.cubeMapCache = NULL;
    memoryCaches.cubeMapCount = 0;
    memoryCaches.textureCache = NULL;
    memoryCaches.texturesCount = 0;
    memoryCaches.modelCache = NULL;
    memoryCaches.modelsCount = 0;
    memoryCaches.shaderCache = NULL;
    memoryCaches.shadersCount = 0;
}

void free_shaders() {
    free(memoryCaches.shaderCache);
    memoryCaches.shadersCount = 0;
    printf("Free shaders!\n");
}

void free_models() {
    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        Model *model = memoryCaches.modelCache[i].model;
        if (model) {
            for (int j = 0; j < model->length; j++) {
                for (int k = 0; k < model->objects[j].materialsCount; k++) {
                    glDeleteLists(model->objects[j].displayLists[k], 1);
                }
                free(model->objects[j].displayLists);
                glDeleteVertexArrays(1, &model->objects[j].VAO);
                free(model->objects[j].materials);
                free(model->objects[j].materialsLength);
            }
            free(model->objects);
            free(model->materials);
        }
    }
    free(memoryCaches.modelCache);
    memoryCaches.modelsCount = 0;
    printf("Free models!\n");
}

void free_textures() {
    for (int i = 0; i < memoryCaches.texturesCount; i++) {
        TextureMap texture = memoryCaches.textureCache[i].textureMap;
        glDeleteTextures(1, &texture);
    }
    free(memoryCaches.textureCache);
    memoryCaches.texturesCount = 0;
    printf("Free textures!\n");
}

void free_cubemaps() {
    for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
        TextureMap texture = memoryCaches.cubeMapCache[i].cubeMap;
        glDeleteTextures(1, &texture);
    }
    free(memoryCaches.cubeMapCache);
    memoryCaches.cubeMapCount = 0;
    printf("Free cubemaps!\n");
}

void free_memory_cache() {
    free_models();
    free_textures();
    free_cubemaps();
    free_shaders();
}
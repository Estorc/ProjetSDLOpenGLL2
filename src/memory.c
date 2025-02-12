#include "raptiquax.h"
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

ModelData * get_model_from_cache(const char *name) {
    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        if (!strcmp(memoryCaches.modelCache[i].modelName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Model loaded from cache!\n");
            #endif
            return memoryCaches.modelCache[i].model;
        }
    }
    return NULL;
}

void add_model_to_cache(const char *name, ModelData *model) {
    memoryCaches.modelCache = realloc(memoryCaches.modelCache, sizeof(ModelCache) * (memoryCaches.modelsCount + 1));
    memoryCaches.modelCache[memoryCaches.modelsCount].model = model;
    strcpy(memoryCaches.modelCache[memoryCaches.modelsCount].modelName, name);
    memoryCaches.modelsCount++;
    #ifdef DEBUG
        PRINT_INFO("Model added to cache!\n");
    #endif
}

void free_models() {
    for (int i = 0; i < memoryCaches.modelsCount; i++) {
        ModelData *model = memoryCaches.modelCache[i].model;
        if (model) {
            for (int j = 0; j < model->length; j++) {
                for (int k = 0; k < model->objects[j].materialsCount; k++) {
                    glDeleteLists(model->objects[j].displayLists[k], 1);
                }
                free(model->objects[j].displayLists);
                glDeleteVertexArrays(1, &model->objects[j].VAO);
                free(model->objects[j].materials);
                free(model->objects[j].materialsLength);
                free(model->objects[j].facesVertex);
            }
            free(model->objects);
        }
    }
    free(memoryCaches.modelCache);
    memoryCaches.modelsCount = 0;
    PRINT_INFO("Free models!\n");
}


Mix_Music * get_mix_music_from_cache(const char *name) {
    for (int i = 0; i < memoryCaches.musicCount; i++) {
        if (!strcmp(memoryCaches.musicCache[i].musicName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Mix Music loaded from cache!\n");
            #endif
            return memoryCaches.musicCache[i].music;
        }
    }
    return NULL;
}

void add_mix_music_to_cache(const char *name, Mix_Music *music) {
    memoryCaches.musicCache = realloc(memoryCaches.musicCache, sizeof(MixMusicCache) * (memoryCaches.musicCount + 1));
    memoryCaches.musicCache[memoryCaches.musicCount].music = music;
    strcpy(memoryCaches.musicCache[memoryCaches.musicCount].musicName, name);
    memoryCaches.musicCount++;
    #ifdef DEBUG
        PRINT_INFO("Mix Music added to cache!\n");
    #endif
}

void free_musics() {
    Mix_HaltMusic();
    for (int i = 0; i < memoryCaches.musicCount; i++) {
        Mix_Music * music = memoryCaches.musicCache[i].music;
        if (music) {
            Mix_FreeMusic(music);
        }
    }
    free(memoryCaches.musicCache);
    memoryCaches.musicCount = 0;
    PRINT_INFO("Free musics!\n");
}


Mix_Chunk * get_mix_chunk_from_cache(const char *name) {
    for (int i = 0; i < memoryCaches.chunkCount; i++) {
        if (!strcmp(memoryCaches.chunkCache[i].chunkName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Mix Chunk loaded from cache!\n");
            #endif
            return memoryCaches.chunkCache[i].chunk;
        }
    }
    return NULL;
}

void add_mix_chunk_to_cache(const char *name, Mix_Chunk *chunk) {
    memoryCaches.chunkCache = realloc(memoryCaches.chunkCache, sizeof(MixChunkCache) * (memoryCaches.chunkCount + 1));
    memoryCaches.chunkCache[memoryCaches.chunkCount].chunk = chunk;
    strcpy(memoryCaches.chunkCache[memoryCaches.chunkCount].chunkName, name);
    memoryCaches.chunkCount++;
    #ifdef DEBUG
        PRINT_INFO("Mix Chunk added to cache!\n");
    #endif
}

void free_chunks() {
    for (int i = 0; i < memoryCaches.chunkCount; i++) {
        Mix_Chunk * chunk = memoryCaches.chunkCache[i].chunk;
        if (chunk) {
            Mix_FreeChunk(chunk);
        }
    }
    free(memoryCaches.chunkCache);
    memoryCaches.chunkCount = 0;
    PRINT_INFO("Free chunks!\n");
}

void free_shaders() {
    free(memoryCaches.shaderCache);
    memoryCaches.shadersCount = 0;
    PRINT_INFO("Free shaders!\n");
}

void free_textures() {
    for (int i = 0; i < memoryCaches.texturesCount; i++) {
        TextureMap texture = memoryCaches.textureCache[i].textureMap;
        glDeleteTextures(1, &texture);
    }
    free(memoryCaches.textureCache);
    memoryCaches.texturesCount = 0;
    PRINT_INFO("Free textures!\n");
}

void free_cubemaps() {
    for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
        TextureMap texture = memoryCaches.cubeMapCache[i].cubeMap;
        glDeleteTextures(1, &texture);
    }
    free(memoryCaches.cubeMapCache);
    memoryCaches.cubeMapCount = 0;
    PRINT_INFO("Free cubemaps!\n");
}

void free_memory_cache() {
    free_chunks();
    free_musics();
    free_models();
    free_textures();
    free_cubemaps();
    free_shaders();
}
#include "raptiquax.h"
#include "math/math_util.h"
#include "io/model.h"
#include "io/shader.h"
#include "memory.h"

void init_memory_cache() {
    Game.memoryCaches->cubeMapCache = NULL;
    Game.memoryCaches->cubeMapCount = 0;
    Game.memoryCaches->textureCache = NULL;
    Game.memoryCaches->texturesCount = 0;
    Game.memoryCaches->modelCache = NULL;
    Game.memoryCaches->modelsCount = 0;
    Game.memoryCaches->shaderCache = NULL;
    Game.memoryCaches->shadersCount = 0;
}

ModelData * get_model_from_cache(const char *name) {
    for (int i = 0; i < Game.memoryCaches->modelsCount; i++) {
        if (!strcmp(Game.memoryCaches->modelCache[i].modelName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Model loaded from cache!\n");
            #endif
            return Game.memoryCaches->modelCache[i].model;
        }
    }
    return NULL;
}

void add_model_to_cache(const char *name, ModelData *model) {
    Game.memoryCaches->modelCache = realloc(Game.memoryCaches->modelCache, sizeof(ModelCache) * (Game.memoryCaches->modelsCount + 1));
    Game.memoryCaches->modelCache[Game.memoryCaches->modelsCount].model = model;
    strcpy(Game.memoryCaches->modelCache[Game.memoryCaches->modelsCount].modelName, name);
    Game.memoryCaches->modelsCount++;
    #ifdef DEBUG
        PRINT_INFO("Model added to cache!\n");
    #endif
}

void free_models() {
    for (int i = 0; i < Game.memoryCaches->modelsCount; i++) {
        ModelData *model = Game.memoryCaches->modelCache[i].model;
        if (model) {
            for (int j = 0; j < model->length; j++) {
                glDeleteVertexArrays(1, &model->objects[j].VAO);
                free(model->objects[j].materials);
                free(model->objects[j].materialsLength);
                free(model->objects[j].facesVertex);
            }
            free(model->objects);
        }
    }
    free(Game.memoryCaches->modelCache);
    Game.memoryCaches->modelsCount = 0;
    PRINT_INFO("Free models!\n");
}


Mix_Music * get_mix_music_from_cache(const char *name) {
    for (int i = 0; i < Game.memoryCaches->musicCount; i++) {
        if (!strcmp(Game.memoryCaches->musicCache[i].musicName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Mix Music loaded from cache!\n");
            #endif
            return Game.memoryCaches->musicCache[i].music;
        }
    }
    return NULL;
}

void add_mix_music_to_cache(const char *name, Mix_Music *music) {
    Game.memoryCaches->musicCache = realloc(Game.memoryCaches->musicCache, sizeof(MixMusicCache) * (Game.memoryCaches->musicCount + 1));
    Game.memoryCaches->musicCache[Game.memoryCaches->musicCount].music = music;
    strcpy(Game.memoryCaches->musicCache[Game.memoryCaches->musicCount].musicName, name);
    Game.memoryCaches->musicCount++;
    #ifdef DEBUG
        PRINT_INFO("Mix Music added to cache!\n");
    #endif
}

void free_musics() {
    Mix_HaltMusic();
    for (int i = 0; i < Game.memoryCaches->musicCount; i++) {
        Mix_Music * music = Game.memoryCaches->musicCache[i].music;
        if (music) {
            Mix_FreeMusic(music);
        }
    }
    free(Game.memoryCaches->musicCache);
    Game.memoryCaches->musicCount = 0;
    PRINT_INFO("Free musics!\n");
}


Mix_Chunk * get_mix_chunk_from_cache(const char *name) {
    for (int i = 0; i < Game.memoryCaches->chunkCount; i++) {
        if (!strcmp(Game.memoryCaches->chunkCache[i].chunkName, name)) {
            #ifdef DEBUG
                PRINT_INFO("Mix Chunk loaded from cache!\n");
            #endif
            return Game.memoryCaches->chunkCache[i].chunk;
        }
    }
    return NULL;
}

void add_mix_chunk_to_cache(const char *name, Mix_Chunk *chunk) {
    Game.memoryCaches->chunkCache = realloc(Game.memoryCaches->chunkCache, sizeof(MixChunkCache) * (Game.memoryCaches->chunkCount + 1));
    Game.memoryCaches->chunkCache[Game.memoryCaches->chunkCount].chunk = chunk;
    strcpy(Game.memoryCaches->chunkCache[Game.memoryCaches->chunkCount].chunkName, name);
    Game.memoryCaches->chunkCount++;
    #ifdef DEBUG
        PRINT_INFO("Mix Chunk added to cache!\n");
    #endif
}

void free_chunks() {
    for (int i = 0; i < Game.memoryCaches->chunkCount; i++) {
        Mix_Chunk * chunk = Game.memoryCaches->chunkCache[i].chunk;
        if (chunk) {
            Mix_FreeChunk(chunk);
        }
    }
    free(Game.memoryCaches->chunkCache);
    Game.memoryCaches->chunkCount = 0;
    PRINT_INFO("Free chunks!\n");
}

void free_shaders() {
    free(Game.memoryCaches->shaderCache);
    Game.memoryCaches->shadersCount = 0;
    PRINT_INFO("Free shaders!\n");
}

void free_textures() {
    for (int i = 0; i < Game.memoryCaches->texturesCount; i++) {
        TextureMap texture = Game.memoryCaches->textureCache[i].textureMap;
        glDeleteTextures(1, &texture);
    }
    free(Game.memoryCaches->textureCache);
    Game.memoryCaches->texturesCount = 0;
    PRINT_INFO("Free textures!\n");
}

void free_cubemaps() {
    for (int i = 0; i < Game.memoryCaches->cubeMapCount; i++) {
        TextureMap texture = Game.memoryCaches->cubeMapCache[i].cubeMap;
        glDeleteTextures(1, &texture);
    }
    free(Game.memoryCaches->cubeMapCache);
    Game.memoryCaches->cubeMapCount = 0;
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
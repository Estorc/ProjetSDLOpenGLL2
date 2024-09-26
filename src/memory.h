#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    TextureMap cubeMap;
    char textureName[6][100];
} CubeMapCache;

typedef struct {
    TextureMap textureMap;
    char textureName[100];
} TextureCache;

typedef struct {
    Model *model;
    char modelName[100];
} ModelCache;

typedef struct {
    Shader shader;
    char shaderName[2][100];
} ShaderCache;

typedef struct {
    CubeMapCache *cubeMapCache;
    int cubeMapCount;
    TextureCache *textureCache;
    int texturesCount;
    ModelCache *modelCache;
    int modelsCount;
    ShaderCache *shaderCache;
    int shadersCount;
} MemoryCaches;

extern MemoryCaches memoryCaches;

#endif

void init_memory_cache();
void free_shaders();
void free_models();
void free_textures();
void free_cubemaps();
void free_memory_cache();
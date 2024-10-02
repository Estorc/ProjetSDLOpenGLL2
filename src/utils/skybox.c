#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../io/shader.h"
#include "../memory.h"
#include "../io/stringio.h"

TextureMap load_cubemap(char faces[6][100]) {
    char *paths[6];
    for (int i = 0; i < 6; i++) {
        paths[i] = relative_path(faces[i]);
    }
    for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
        if (!strcmp(memoryCaches.cubeMapCache[i].textureName[0], paths[0]) &&
            !strcmp(memoryCaches.cubeMapCache[i].textureName[1], paths[1]) &&
            !strcmp(memoryCaches.cubeMapCache[i].textureName[2], paths[2]) &&
            !strcmp(memoryCaches.cubeMapCache[i].textureName[3], paths[3]) &&
            !strcmp(memoryCaches.cubeMapCache[i].textureName[4], paths[4]) &&
            !strcmp(memoryCaches.cubeMapCache[i].textureName[5], paths[5])) {
            #ifdef DEBUG
                printf("Cube Map loaded from cache!\n");
            #endif
            return memoryCaches.cubeMapCache[i].cubeMap;
        }
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int success = 1;

    for (unsigned int i = 0; i < 6; i++) {
        SDL_Surface* textureSurface = IMG_Load(paths[i]);

        if (textureSurface) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels
            );
            SDL_FreeSurface(textureSurface);
        } else {
            printf("Cubemap tex failed to load at path: %s\n", paths[i]);
            SDL_FreeSurface(textureSurface);
            success = 0;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (success) {
        memoryCaches.cubeMapCache = realloc(memoryCaches.cubeMapCache, sizeof (CubeMapCache) * (++memoryCaches.cubeMapCount));
        memoryCaches.cubeMapCache[memoryCaches.cubeMapCount-1].cubeMap = textureID;
        for (unsigned int i = 0; i < 6; i++) {
            strcpy(memoryCaches.cubeMapCache[memoryCaches.cubeMapCount-1].textureName[i], paths[i]);
        }
    }
    for (int i = 0; i < 6; i++) free(paths[i]);
    return textureID;
}  


void create_skybox(TexturedMesh *texturedMesh, char skyboxTexture[6][100]) {

    VBO skyboxVBO;
    VAO skyboxVAO;

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    // fill buffer
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    // link vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texturedMesh->VAO = skyboxVAO;
    texturedMesh->length = 36;
    texturedMesh->texture = load_cubemap(skyboxTexture);

}
#include <stdlib.h>
#include <stdio.h>
#include "../types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <limits.h>

#include "../math/math_util.h"
#include "model.h"
#include "input.h"
#include "gltexture_loader.h"
#include "shader.h"
#include "stringio.h"
#include "../memory.h"



/**
 * Loads a texture from a specified file path and creates an OpenGL texture.
 *
 * @param path {char*} A string containing the file path to the texture image.
 * @return {TextureMap} The generated OpenGL texture ID, or 0 if loading fails.
 *
 * This function utilizes SDL_image to load a texture from the specified file 
 * path, creates an OpenGL texture object, and configures its parameters. The 
 * loaded texture is suitable for use in rendering graphics with OpenGL.
 *
 * Example Usage:
 * TextureMap texture = load_texture_from_path("path/to/texture.png");
 * if (texture != 0) {
 *     // Use the texture for rendering...
 * }
 *
 * Return:
 * Returns the OpenGL texture ID on success. If the texture fails to load, 
 * the function returns 0.
 */

TextureMap load_texture_from_path(char * path) {

    for (int i = 0; i < memoryCaches.texturesCount; i++) {
        if (!strcmp(memoryCaches.textureCache[i].textureName, path)) {
            #ifdef DEBUG
                printf("Texture loaded from cache!\n");
            #endif
            return memoryCaches.textureCache[i].textureMap;
        }
    }
    SDL_Surface* textureSurface = IMG_Load(path);
    TextureMap texture = 0;

    if (textureSurface) {
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, textureSurface->w, textureSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface->pixels);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        memoryCaches.textureCache = realloc(memoryCaches.textureCache, sizeof (TextureCache) * (++memoryCaches.texturesCount));
        memoryCaches.textureCache[memoryCaches.texturesCount-1].textureMap = texture;
        strcpy(memoryCaches.textureCache[memoryCaches.texturesCount-1].textureName, path);
        
    } else {
        printf("Failed to load texture : %s\n", SDL_GetError());
    }

    SDL_FreeSurface(textureSurface);
    return texture;
}



void draw_sprite(SDL_Surface *render_surface, int x, int y, char * path, u32 color) {
    SDL_Surface* textureSurface = IMG_Load(path);

    if (textureSurface) {
        SDL_Rect spriteLocation = { x, y, 0, 0 };
        SDL_SetSurfaceColorMod(textureSurface, (color & 0xff0000) >> 16, (color & 0x00ff00) >> 8, (color & 0x0000ff));
        SDL_SetSurfaceAlphaMod(textureSurface, (color & 0xff000000) >> 24);
        SDL_BlitSurface(textureSurface, NULL, render_surface, &spriteLocation);
    } else
        printf("Failed to load texture : %s\n", SDL_GetError());

    SDL_FreeSurface(textureSurface);
}

int draw_input_box(SDL_Surface *render_surface, int x, int y, int width, int height, u32 color, Input *input) {
    SDL_Rect rect = {x, y, width, height};
    if (input->mouse.x > x &&
        input->mouse.x < x+width &&
        input->mouse.y > y &&
        input->mouse.y < y+height) {
        SDL_FillRect(render_surface, &rect, color);
        if (input->mouse.pressed_button) {
            SDL_StartTextInput();
            strcpy(input->inputBuffer, "");
            return 1;
        }
    } else
        SDL_FillRect(render_surface, &rect, color & 0xa0ffffff);
    return 0;
}

int draw_button(SDL_Surface *render_surface, int x, int y, int width, int height, char * path, u32 color, Input *input) {
    if (input->mouse.x > x &&
        input->mouse.x < x+width &&
        input->mouse.y > y &&
        input->mouse.y < y+height) {
        draw_sprite(render_surface, x, y, path, color);
        return (input->mouse.pressed_button);
    } else
        draw_sprite(render_surface, x, y, path, color & 0xa0ffffff);
    return 0;
}

void draw_text(SDL_Surface *render_surface, int x, int y, char *text, TTF_Font *font, SDL_Color color) {
    SDL_Surface *surface;

    surface = TTF_RenderText_Blended_Wrapped(font, text, color, INT_MAX);
    SDL_Rect textLocation = { x, y, 0, 0 };
    SDL_BlitSurface(surface, NULL, render_surface, &textLocation);
    SDL_FreeSurface(surface);
}

void draw_rectangle(SDL_Surface *render_surface, int x, int y, int width, int height, u32 color) {
    SDL_Rect rect = {x, y, width, height};
    SDL_FillRect(render_surface, &rect, color);
}
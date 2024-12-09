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


float * convert_to_rgb32f_texture(SDL_Surface *rgbaSurface) {
    // Get the pixel data
    Uint32* pixels = (Uint32*)rgbaSurface->pixels;
    int width = rgbaSurface->w;
    int height = rgbaSurface->h;

    // Allocate memory for the OpenGL texture (float array)
    float* textureData = (float*)malloc(3 * width * height * sizeof(float));

    // Convert each pixel to floating point
    for (int i = 0; i < width * height; ++i) {
        Uint32 pixel = pixels[i];
        Uint8 r = (pixel >> 24) & 0xFF;
        Uint8 g = (pixel >> 16) & 0xFF;
        Uint8 b = (pixel >> 8) & 0xFF;

        // Convert to floating point [0, 1]
        textureData[i * 3 + 0] = r / 255.0f;
        textureData[i * 3 + 1] = g / 255.0f;
        textureData[i * 3 + 2] = b / 255.0f;
    }
    return textureData;
}

SDL_Surface * flip_y_surface(SDL_Surface *surface) {
    SDL_Surface *flippedSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
            ((Uint32*)flippedSurface->pixels)[(surface->h - y - 1) * surface->w + x] = pixel;
        }
    }
    return flippedSurface;
}


TextureMap load_texture_from_path(char * path, GLenum format) {

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

    if (!textureSurface) {
        printf("Failed to load texture : %s\n", SDL_GetError());
        return 0;
    }

    GLenum externalFormat;
    GLenum dataType;
    float * fTextureData = NULL;
    SDL_Surface* formattedSurface;
    switch (format) {
        case GL_RGB32F:
            formattedSurface = SDL_ConvertSurfaceFormat(textureSurface, SDL_PIXELFORMAT_RGBA8888, 0);
            externalFormat = GL_RGB;
            dataType = GL_FLOAT;
            break;
        default:
            formattedSurface = SDL_ConvertSurfaceFormat(textureSurface, SDL_PIXELFORMAT_ABGR8888, 0);
            externalFormat = GL_RGBA;
            dataType = GL_UNSIGNED_BYTE;
            break;
    }
    SDL_Surface* correctedSurface = flip_y_surface(formattedSurface);

    if (format == GL_RGB32F) {
        fTextureData = convert_to_rgb32f_texture(correctedSurface);
    }
    
    if (!formattedSurface) {
        printf("Failed to convert surface: %s\n", SDL_GetError());
        return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, correctedSurface->w, correctedSurface->h, 0, externalFormat, dataType, (fTextureData) ? fTextureData : correctedSurface->pixels);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    memoryCaches.textureCache = realloc(memoryCaches.textureCache, sizeof (TextureCache) * (++memoryCaches.texturesCount));
    memoryCaches.textureCache[memoryCaches.texturesCount-1].textureMap = texture;
    strcpy(memoryCaches.textureCache[memoryCaches.texturesCount-1].textureName, path);
    

    SDL_FreeSurface(textureSurface);
    SDL_FreeSurface(correctedSurface);
    SDL_FreeSurface(formattedSurface);
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
#include "../raptiquax.h"
#include "../math/math_util.h"
#include "model.h"
#include "input.h"
#include "gltexture_loader.h"
#include "shader.h"
#include "../memory.h"



float * convert_to_rgb32f_texture(SDL_Surface *rgbaSurface) {
    // Get the pixel data
    Uint32* pixels = (Uint32*)rgbaSurface->pixels;
    int width = rgbaSurface->w;
    int height = rgbaSurface->h;

    // Allocate memory for the OpenGL texture (float array)
    float* textureData = (float*)malloc(3 * width * height * sizeof(float));

    // Convert each pixel to floating point
    Uint32 pixel;
    Uint8 r, g, b;
    float inv255 = 1.0f / 255.0f;
    for (int i = 0; i < width * height; ++i) {
        pixel = pixels[i];
        r = (pixel >> 24) & 0xFF;
        g = (pixel >> 16) & 0xFF;
        b = (pixel >> 8) & 0xFF;

        // Convert to floating point [0, 1]
        textureData[i * 3 + 0] = r * inv255;
        textureData[i * 3 + 1] = g * inv255;
        textureData[i * 3 + 2] = b * inv255;
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


TextureMap load_texture_from_path(char * path, GLenum format, bool yReversed) {

    for (int i = 0; i < Game.memoryCaches->texturesCount; i++) {
        if (!strcmp(Game.memoryCaches->textureCache[i].textureName, path)) {
            #ifdef DEBUG
                PRINT_INFO("Texture loaded from cache!\n");
            #endif
            return Game.memoryCaches->textureCache[i].textureMap;
        }
    }
    SDL_Surface* textureSurface = IMG_Load(path);
    TextureMap texture = 0;

    if (!textureSurface) {
        PRINT_ERROR("Failed to load texture %s : %s\n", path, SDL_GetError());
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
    SDL_Surface* correctedSurface;
    if (yReversed) correctedSurface = flip_y_surface(formattedSurface);
    else correctedSurface = formattedSurface;

    if (format == GL_RGB32F) {
        fTextureData = convert_to_rgb32f_texture(correctedSurface);
    }
    
    if (!formattedSurface) {
        PRINT_ERROR("Failed to convert surface: %s\n", SDL_GetError());
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

    Game.memoryCaches->textureCache = realloc(Game.memoryCaches->textureCache, sizeof (TextureCache) * (++Game.memoryCaches->texturesCount));
    Game.memoryCaches->textureCache[Game.memoryCaches->texturesCount-1].textureMap = texture;
    strcpy(Game.memoryCaches->textureCache[Game.memoryCaches->texturesCount-1].textureName, path);
    

    if (yReversed) SDL_FreeSurface(correctedSurface);
    SDL_FreeSurface(textureSurface);
    SDL_FreeSurface(formattedSurface);
    free(fTextureData);
    return texture;
}


void draw_text(SDL_Surface *render_surface, int x, int y, const char *text, TTF_Font *font, SDL_Color color, char *alignment, int width) {
    SDL_Surface *surface;

    char *buffer = malloc(strlen(text) + 1);
    buffer[0] = 0;
    strcpy(buffer, text);
    char *line = buffer;
    char *nl_pos;

    while (line) {
        if (!(*line)) line++;
        nl_pos = strchr(line, '\n');
        if (nl_pos) *nl_pos = 0;
        int dx, dy;
        if (*line) {
            surface = TTF_RenderUTF8_Blended_Wrapped(font, line, color, (width == -1) ? INT_MAX : width);
            if (!surface) {
                PRINT_ERROR("Failed to render text: %s\n", TTF_GetError());
                return;
            }
            switch (alignment[0]) {
                case 'l':
                    dx = x;
                    break;
                case 'r':
                    dx = x + render_surface->w-surface->w;
                    break;
                case 'c':
                    dx = x + (render_surface->w-surface->w) / 2;
                    break;
            }
            switch (alignment[1]) {
                case 't':
                    dy = y;
                    break;
                case 'b':
                    dy = y + render_surface->h-surface->h;
                    break;
                case 'c':
                    dy = y + (render_surface->h-surface->h) / 2;
                    break;
            }
            SDL_Rect textLocation = { dx, dy, 0, 0 };
            y += surface->h;
            SDL_BlitSurface(surface, NULL, render_surface, &textLocation);
            SDL_FreeSurface(surface);
        } else {
            y += TTF_FontHeight(font);
        }
        line = nl_pos;
    }
    free(buffer);
}












void draw_sprite(SDL_Surface *render_surface, int x, int y, char * path, u32 color) {
    SDL_Surface* textureSurface = IMG_Load(path);

    if (textureSurface) {
        SDL_Rect spriteLocation = { x, y, 0, 0 };
        SDL_SetSurfaceColorMod(textureSurface, (color & 0xff0000) >> 16, (color & 0x00ff00) >> 8, (color & 0x0000ff));
        SDL_SetSurfaceAlphaMod(textureSurface, (color & 0xff000000) >> 24);
        SDL_BlitSurface(textureSurface, NULL, render_surface, &spriteLocation);
    } else
        PRINT_ERROR("Failed to load texture : %s\n", SDL_GetError());

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


void draw_rectangle(SDL_Surface *render_surface, int x, int y, int width, int height, u32 color) {
    SDL_Rect rect = {x, y, width, height};
    SDL_FillRect(render_surface, &rect, color);
}
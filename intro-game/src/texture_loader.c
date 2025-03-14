#include "../include/lib.h"
#include "../include/texture_loader.h"

// load png file and return the texture 
SDL_Texture * load_png (char * path) {
    SDL_Surface * surface = IMG_Load (path);
    if (surface == NULL) {
        printf("Erreur de chargement de l'image: %s\n", IMG_GetError());
        return NULL;
    }
    
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Erreur creation texture : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}
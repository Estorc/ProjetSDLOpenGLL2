#pragma once 

#include "lib.h"
 
extern SDL_Renderer * renderer ;
typedef struct Dictionary_u Dictionary_t ;

typedef struct Texture_u {
    SDL_Texture * texture ;
    SDL_Rect position ;
    SDL_Rect srcrect ;
    
    int hidden ;
} Texture_t ;

Texture_t * create_texture (SDL_Rect position, SDL_Rect srcrect, int hidden) ;
void destroy_texture (Texture_t ** texture) ;
void destroy_texture_cb (void * texture) ;

// return NULL if failed, print the error message in this case 
SDL_Texture * load_png (char * path) ;
void SDL_DestroyTexture_cb (void * texture) ;
int load_textures_from_file (Dictionary_t * dict, const char * dataPath) ;

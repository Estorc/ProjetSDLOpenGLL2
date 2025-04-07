#pragma once 


#include "physics.h"
#include "window.h"

typedef struct List_u List_t ;
typedef struct Texture_u Texture_t ; 

extern GameStatus_t gameStatus ;

#define BACKGROUND_WIDTH 2304
#define BACKGROUND_HEIGHT 1296

#define GROUND_WIDTH BACKGROUND_WIDTH
#define GROUND_HEIGHT (WINDOW_HEIGHT / 4)

typedef struct MapObj_u {
    Texture_t sprite ;
    uint8_t idSpriteSheet ;
} MapObj_t ;

typedef struct Map_u {
    SDL_Texture * background ;

    SDL_FRect ground ;

    List_t * listObjects ;
} Map_t ;


Map_t * map_constructor () ;
void map_destructor (Map_t ** map) ;
void map_destructor_cb (void * map) ;
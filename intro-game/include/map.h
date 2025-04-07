#pragma once 


#include "physics.h"
#include "window.h"

extern GameStatus_t gameStatus ;

#define BACKGROUND_WIDTH 2304
#define BACKGROUND_HEIGHT 1296

#define GROUND_WIDTH BACKGROUND_WIDTH
#define GROUND_HEIGHT (WINDOW_HEIGHT / 4)

#define ICON_LOCK 0
#define ICON_GAME 1
#define ICON_DOC 2
#define ERROR_WINDOW 3
#define TOP_BORDER 4
#define LEFT_BORDER 5
#define RIGHT_BORDER 6
#define BOTTOM_BORDER 7

typedef struct DesktopElement_u {
    SDL_Rect srcrect ;
    SDL_Rect position ;
    int hidden ;        // TRUE or FALSE 
    int clicked ;       // TRUE or FALSE 
    int dragged ;       // TRUE or FALSE 
} DesktopElement_t ; 

typedef struct Map_u {
    SDL_Texture * background ;

    SDL_FRect ground ;
    
    SDL_Texture * objectsTexture ;
    DesktopElement_t * listElements ;
    int nbObject;
} Map_t ;


DesktopElement_t * load_elements(int nbObject);
Map_t * map_constructor () ;
void map_destructor (Map_t ** map) ;
void map_destructor_cb (void * map) ;
void init_elements_scene0 (Map_t * map);
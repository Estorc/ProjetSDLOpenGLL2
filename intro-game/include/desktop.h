#pragma once 

#include "lib.h"


typedef struct DesktopElementt_u {
    SDL_Rect srcrect ;
    SDL_Rect position ;
    int hidden ;        // TRUE or FALSE 
    int clicked ;       // TRUE or FALSE 
    int dragged ;       // TRUE or FALSE 
} DesktopElementt_t ; 

typedef struct Desktop_u {
    SDL_Texture * background ;

    SDL_Texture * elementSpritesheet ;
    DesktopElementt_t * elements ;
    int nbElem ;
} Desktop_t ;


Desktop_t * create_desktop (char * backgroundPath, char * elementSpritesheetPath, char * dataPath, int nbElements) ;
void destroy_desktop(Desktop_t **desktop) ;
int element_update(Desktop_t * desktop, SDL_Event * event) ;
void move_element (Desktop_t * desktop, SDL_Event * event) ;
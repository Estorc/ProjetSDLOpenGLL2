#include "../include/lib.h"
#include "../include/main.h"
#include "../include/commun.h"

int existe (void * ptr) {
    if (ptr != NULL) 
        return TRUE ;
    else 
        return FALSE ;
}

int compare_SDL_Color (SDL_Color a, SDL_Color b) {

    int sommeA = a.r + a.g + a.b + a.a ;
    int sommeB = b.r + b.g + b.b + b.a ;

    return sommeA - sommeB ;
}

int compare_SDL_Rect (SDL_Rect a, SDL_Rect b) {

    int sommeA = a.x + a.y + a.w + a.h ;
    int sommeB = b.x + b.y + b.w + b.h ;

    return sommeA - sommeB ;
}
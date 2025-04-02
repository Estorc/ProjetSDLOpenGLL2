#pragma once 

#include "lib.h"

int existe (void * ptr) ;
int compare_SDL_Color (SDL_Color a, SDL_Color b) ;
int compare_SDL_Rect (SDL_Rect a, SDL_Rect b) ;
int fileModified (const char * path) ;
void free_cb (void * data) ;
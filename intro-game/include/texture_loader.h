#pragma once 

#include "lib.h"

extern SDL_Renderer * renderer ;

// return NULL if failed, print the error message in this case 
SDL_Texture * load_png (char * path);
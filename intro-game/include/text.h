#pragma once 


#include "lib.h"
#include "render.h" 


typedef struct Text_u { 
    TTF_Font * font ;               // Police de caractère
    char * string ;                 // Texte à afficher
    int len ;                       // taille string 

    TextAnim_t animation ;          // Structure propre à l'animation 
} Text_t ; 


Text_t * create_text (const char * string, TTF_Font * font, SDL_Color * color, SDL_Rect * position) ;
void destroy_text (Text_t ** text) ;
SDL_Texture * create_TTF_Texture (TTF_Font * font, const char * string, SDL_Color color) ;
void text_update (Text_t * text) ;
void text_change_hollow (Text_t * text, int boolean, SDL_Color color, TypeHollow_t dir) ;
void text_change_color (Text_t * text, SDL_Color newColor) ;
void text_change_position (Text_t * text, SDL_Rect newPosition) ; 
void text_change_type_anim (Text_t * text, TypeTextAnim_t type) ;

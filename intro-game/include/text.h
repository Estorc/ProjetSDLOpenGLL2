#pragma once 


#include "lib.h"
#include "render.h" 

typedef struct Dictionary_u Dictionary_t ;
typedef struct List_u List_t ;

typedef struct Text_u { 
    TTF_Font * font ;               // Police de caractère
    char * string ;                 // Texte à afficher
    int len ;                       // taille string 

    TextAnim_t animation ;          // Structure propre à l'animation  
    int hidden ;                    // texte affiche ou non 
} Text_t ; 


Text_t * create_text (const char * string, TTF_Font * font, SDL_Color color, SDL_Rect position) ;
void destroy_text (Text_t ** text) ;
void destroy_text_cb (void * text) ;
SDL_Texture * create_TTF_Texture (TTF_Font * font, const char * string, SDL_Color color) ;
void text_update (Text_t * text) ;
void text_dict_update (Dictionary_t * dict, const char * dataPath) ;
void text_change_hollow (Text_t * text, int boolean, SDL_Color color, TypeHollow_t dir) ;
void text_change_color (Text_t * text, SDL_Color newColor) ;
void text_change_position (Text_t * text, SDL_Rect newPosition) ; 
void text_change_type_anim (Text_t * text, TypeTextAnim_t newType) ;
void text_change_font (Text_t * text, TTF_Font * newFont) ;
void text_change_visibility (Text_t * text, int hidden) ;
void TTF_CloseFont_cb (void * font) ;

int load_texts_from_file (const char * dataPath, List_t * font, Dictionary_t * dict) ;
void get_text_dimensions (Text_t * text, int * w, int * h) ;
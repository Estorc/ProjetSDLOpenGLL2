#pragma once 

#include "lib.h"
 
extern SDL_Renderer * renderer ;
typedef struct Dictionary_u Dictionary_t ;
typedef struct List_u List_t ;

/**
 * Définis le type d'animation que subira la texture : 
 * - NONE : comme son nom l'indique la texture sera afiché en tant que telle sans modification au cours du temps
 * - DEFAULT : considere la texture comme une suite de frame de meme largeur et hauteur. en fonction de la frame actuelle
 *   affcihera une portion differente de la texture (srcrect.x = srcrect.w * texture->currentFrame).
 * - ROTATION : fais tourner dans le sens des aiguilles d'une montre la texture. angle de rotation calculer comme suit :
 *   angle = (360.0 / texture->numFrames) * texture->currentFrame. plusle nombre de frame sera grand plus la rotation sera fluide.
 *   ne change pas la position du srcrect, affiche donc toujours la meme portion de la texture.
 */
typedef enum TypeTextureAnim_u {NONE, DEFAULT, ROTATION} TypeTextureAnim_t ;

typedef struct Texture_u {
    #include "common_anim_attributes.h"

    TypeTextureAnim_t typeAnim ;

    SDL_Texture * texture ;
    SDL_Rect position ;
    SDL_Rect srcrect ;
    
    int hidden ;

} Texture_t ;

Texture_t * create_texture (SDL_Rect position, SDL_Rect srcrect, int hidden) ;
void destroy_texture (Texture_t ** texture) ;
void destroy_texture_cb (void * texture) ;
void texture_update (Texture_t * texture) ;
void texture_list_update (List_t * list) ;
void texture_list_update_from_file (List_t * list, const char * dataPath) ;
int load_textures_from_file (List_t * list, const char * dataPath) ;

// return NULL if failed, print the error message in this case 
SDL_Texture * load_png (char * path) ;
void SDL_DestroyTexture_cb (void * texture) ;

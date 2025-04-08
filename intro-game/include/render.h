#pragma once 


#include "main.h"
#include "lib.h"
#include "player.h"
#include "map.h"
#include "camera.h" 
#include "gui.h" 

#define TEXT_ANIM_SPEED 6

extern SDL_Renderer * renderer ;
extern GameStatus_t gameStatus ;


typedef struct Text_u Text_t ;
typedef struct Texture_u Texture_t ;
typedef enum Direction_u {UP, DOWN, LEFT, RIGHT} Direction_t ;
typedef enum TypeHollow_u {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT} TypeHollow_t ;
typedef enum TypeTextAnim_u {LEFT_RIGHT} TypeTextAnim_t ; 


typedef struct Anim_u {
    #include "common_anim_attributes.h"
} Anim_t ;

/**
 * Structure de gestion de l'animation pour chacun des objets ou entités devant etre animée.
 */
typedef struct EntityAnim_u {
    #include "common_anim_attributes.h"

    SDL_Texture * spritesheet;      // Texture globale contenant tous les sprites
    int startX, startY;             // Position de la première frame dans le spritesheet
    int frameWidth, frameHeight;    // Taille d'une frame
} EntityAnim_t ;

/**
 * Structure de gestion de l'animation des textes à afficher. 
 */
typedef struct TextAnim_u {
    // Variables propre aux animations 
    #include "common_anim_attributes.h"

    // Texture à afficher 
    SDL_Texture * texture ;         // Message en texture 
    SDL_Color textColor ;           // Couleur du texte 
    SDL_Rect position ;             // Position du message sur l'écran 
    int needChange ;                // TRUE = texture doit etre changé, FALSE = texture à jour 
    
    // Le texte peut avoir une ombre (optionnel)
    int hollow ;                    // TRUE = le texte à une ombre, FALSE = le texte n'a pas d'ombre 
    SDL_Texture * hollowTexture ;   // Contient la texture de l'ombre 
    SDL_Color hollowColor ;         // Couleur de l'ombre 
    TypeHollow_t hollowDir ;         // Dans quelle direction se dirige l'ombre 
    
    int width ;                     // largeur de la zone dans laquelle sera rendu le texte 

    TypeTextAnim_t typeAnim ;
} TextAnim_t ; 
 

err_t draw_text (Text_t * text) ;
void draw_texture (Texture_t * texture, Camera_t * camera) ;
void draw_map (Map_t * map, Camera_t * camera) ;
void draw_desktop (Desktop_t * desktop) ;
int draw (Camera_t * camera, Player_t * player, Map_t * map) ;
void draw_player_pv (Player_t * player, SDL_Texture * texture) ;
void draw_timer (uint32_t time) ;

int generate_noise_texture(int width, int height) ;
void destroy_noise_texture () ;
int generate_desktop_glitch_texture () ;
void destroy_desktop_glitch_texture () ;
void apply_glitch(Camera_t * camera, SDL_Texture * texture) ;

void draw_rect_filled (SDL_Rect rect, SDL_Color color) ;
void draw_rect_border (SDL_Rect rect, SDL_Color color) ;
#pragma once 


#include "physics.h"
#include "window.h"
#include "texture_loader.h"

#define ACT_ID_LOSS_PV 0 

typedef struct List_u List_t ;
typedef struct Player_u Player_t ;
typedef struct Scene_u Scene_t ;

extern GameStatus_t gameStatus ; 

#define BACKGROUND_WIDTH 2304
#define BACKGROUND_HEIGHT 1296

#define GROUND_WIDTH BACKGROUND_WIDTH
#define GROUND_HEIGHT (WINDOW_HEIGHT / 4)

typedef struct MapObj_u {
    uint8_t idSpriteSheet ;
    Texture_t sprite ;
    float vx, vy ;

    uint8_t idAction ;      // action a réaliser quand personnage touche obj (si applicable)
} MapObj_t ;

typedef struct Map_u {
    SDL_Texture * background ;

    SDL_FRect ground ;

    List_t * listSpriteSheet ;    
    List_t * listObjects ;
} Map_t ;


Map_t * map_constructor () ;
void map_destructor (Map_t ** map) ;
void map_destructor_cb (void * map) ;
void map_update (Scene_t * scene, Map_t * map, Player_t * player) ;

MapObj_t * create_mapObj () ;
void destroy_mapObj (MapObj_t ** object) ;
void destroy_mapObj_cb (void * object) ;
void mapObj_action (Scene_t * scene, Map_t * map, Player_t * player, uint8_t idAction) ;
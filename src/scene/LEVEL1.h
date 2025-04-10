#pragma once 


#include "lib.h" 


typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 
typedef struct InfoScene_u InfoScene_t ; 

extern GameStatus_t gameStatus ;
extern uint8_t icon_locked ;

void LEVEL1_load (Scene_t * self) ;
void LEVEL1_unLoad (Scene_t * self) ;
void LEVEL1_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void LEVEL1_update (Scene_t * self, SceneManager_t * manager) ;
void LEVEL1_render (Scene_t * self) ; 
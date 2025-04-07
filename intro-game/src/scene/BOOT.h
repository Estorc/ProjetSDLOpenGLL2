#pragma once 


#include "lib.h"


typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 
typedef struct InfoScene_u InfoScene_t ; 

extern SceneManager_t * sceneManager ;


void BOOT_load (Scene_t * self) ;
void BOOT_unLoad (Scene_t * self) ;
void BOOT_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void BOOT_update (Scene_t * self, SceneManager_t * manager) ;
void BOOT_render (Scene_t * self) ; 
void BOOT_change_state (Scene_t * self, InfoScene_t * info) ;
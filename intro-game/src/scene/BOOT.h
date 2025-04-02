#pragma once 


#include "lib.h"


#define STATE_LOADING 0
#define STATE_BLIS 1
#define STATE_CMD 2

#define I_LOADING_WHEEL 0
#define I_BLIS_FL5 1

typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 
typedef struct InfoScene_u InfoScene_t ; 


void BOOT_load (Scene_t * self) ;
void BOOT_unLoad (Scene_t * self) ;
void BOOT_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void BOOT_update (Scene_t * self, SceneManager_t * manager) ;
void BOOT_render (Scene_t * self) ; 
void BOOT_change_state (Scene_t * self, InfoScene_t * info) ;
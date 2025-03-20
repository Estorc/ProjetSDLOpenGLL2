#pragma once 


#include "lib.h"

#define MAX_EVENT 20 

#define I_LOADING_WHEEL 0
#define I_BLIS_FL5 1

typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 

#define BOOT_NB_EVENT 3
typedef enum BOOT_Event_u {LOADING, BLIS, CMD} BOOT_Event_t ;
void BOOT_load (Scene_t * self) ;
void BOOT_unLoad (Scene_t * self) ;
void BOOT_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void BOOT_update (Scene_t * self, SceneManager_t * manager) ;
void BOOT_render (Scene_t * self) ; 
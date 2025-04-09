#pragma once 

#include "../../include/lib.h"

#define DESKTOP_NB_EVENT 3
#define NB_ELEM_DESKTOP_SCENE 4

typedef struct Camera_u Camera_t ; 
typedef struct Player_u Player_t ;
typedef struct Dictionary_u Dictionary_t ; 
typedef struct Map_u Map_t ; 
typedef struct Scene_u Scene_t ;  
typedef struct SceneManager_u SceneManager_t ; 


// fonctions manip scene desktop
typedef enum DESKTOP_Event_u {D_LOADING, D_BLIS, D_CMD} DESKTOP_Event_t ;
void DESKTOP_load (Scene_t * self) ;
void DESKTOP_unLoad (Scene_t * self) ;
void DESKTOP_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void DESKTOP_update (Scene_t * self, SceneManager_t * manager) ;
void DESKTOP_render (Scene_t * self) ; 


typedef struct Window_u Window_t ;
void window_add_widget_test (Window_t * window) ;
#pragma once 

#include "lib.h"

#define MAX_SCENES 2
#define NB_ELEM_DESKTOP_SCENE 4

extern SDL_Renderer * renderer ;
extern GameStatus_t gameStatus ;

typedef struct Camera_u Camera_t ;
typedef struct Player_u Player_t ;
typedef struct Map_u Map_t ; 
typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ;

typedef struct InfoScene_u {
    int len ;       // Taille attribut `data` de la scene 
    int end ;       // Trigger de la fin du jeu, prends valeur TRUE ou FALSE 
    int nbText ;    // Taille du tableau de text 
} InfoScene_t ;

typedef struct Scene_u {
    char * name ;
    void ** data ;

    void (*load) (Scene_t *) ;
    void (*unLoad) (Scene_t *) ;

    void (*handleEvents) (Scene_t *, SDL_Event *, SceneManager_t *) ;
    void (*update) (Scene_t *, SceneManager_t *) ;
    void (*render) (Scene_t *) ;
} Scene_t ;

typedef struct SceneManager_u {
    Scene_t * scenes[MAX_SCENES] ;      // liste de scenes 
    int sceneCount ;        // nombre de scenes présentes dans la liste de scenes 
    int index ;             // indice de la scene active 
    char nextScene[128] ;      // nom de la prochaine, prends une valeur que si un changement de scene doit se produire 
} SceneManager_t ;


// fonctions manip scene manager
SceneManager_t * create_scene_manager () ;
void destroy_scene_manager (SceneManager_t ** manager) ;
void change_scene (SceneManager_t * manager);
void push_scene (SceneManager_t * manager, Scene_t * newScene) ;
void request_scene_change (SceneManager_t * manager, char * name) ;
int play_scene (SceneManager_t * manager, SDL_Event * event);

// fonctions manip scene_t
Scene_t * create_scene (char * name, void (*load) (Scene_t *), void (*unLoad) (Scene_t *), 
                        void (*handleEvents) (Scene_t *, SDL_Event *, SceneManager_t *), void (*update) (Scene_t *, SceneManager_t *), 
                        void (*render) (Scene_t *));
void destroy_scene (Scene_t ** scene);

// fonctions manip scene desktop
void DESKTOP_load (Scene_t * self) ;
void DESKTOP_unLoad (Scene_t * self) ;
void DESKTOP_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void DESKTOP_update (Scene_t * self, SceneManager_t * manager) ;
void DESKTOP_render (Scene_t * self) ; 

// fonctions manip scene level1
void LEVEL1_load (Scene_t * self) ;
void LEVEL1_unLoad (Scene_t * self) ;
void LEVEL1_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void LEVEL1_update (Scene_t * self, SceneManager_t * manager) ;
void LEVEL1_render (Scene_t * self) ; 
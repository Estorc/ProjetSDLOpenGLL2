#pragma once 


#include "lib.h"
#include "../src/scene/BOOT.h"
#include "../src/scene/DESKTOP.h"
#include "../include/LEVEL1.h"

#define MAX_SCENES 10

#define GET_LIST_TEXTURE(dict) dict->get(dict, "listTexture") 
#define GET_LIST_TEXT(dict) dict->get(dict, "listText") 
#define GET_INFO(dict) dict->get(dict, "info")
#define GET_EVENT_MANAGER(dict) dict->get(dict, "eventManager")

extern SDL_Renderer * renderer ;


typedef struct Dictionary_u Dictionary_t ; 
typedef struct Map_u Map_t ; 
typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 
extern SceneManager_t * sceneManager ;

typedef struct InfoScene_u {
    int len ;       // Taille attribut `data` de la scene 
    int end ;       // Trigger de la fin du jeu, prends valeur TRUE ou FALSE 
    uint32_t startTime ;
    uint32_t currentTime ;
    uint8_t state ;
    uint8_t nextState ;
} InfoScene_t ;

typedef struct Scene_u {
    char * name ;
    Dictionary_t * data ;

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

// fonctions manip scene_t
Scene_t * create_scene (char * name, void (*load) (Scene_t *), void (*unLoad) (Scene_t *), 
            void (*handleEvents) (Scene_t *, SDL_Event *, SceneManager_t *), void (*update) (Scene_t *, SceneManager_t *), 
            void (*render) (Scene_t *));    
void destroy_scene (Scene_t ** scene);
int play_scene (SceneManager_t * manager, SDL_Event * event);
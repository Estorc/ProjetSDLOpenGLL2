#pragma once 

#include "lib.h"
#include "../src/scene/BOOT.h"
#include "../src/scene/DESKTOP.h"

#define MAX_SCENES 10

#define GET_LIST_TEXTURE(dict) dict->get(dict, "listTexture")
#define GET_INFO(dict) dict->get(dict, "info")
#define GET_EVENT_MANAGER(dict) dict->get(dict, "eventManager")

extern SDL_Renderer * renderer ;
extern GameStatus_t gameStatus ;

typedef struct Camera_u Camera_t ; 
typedef struct Player_u Player_t ;
typedef struct Dictionary_u Dictionary_t ; 
typedef struct Map_u Map_t ; 
typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 

typedef struct InfoScene_u {
    int len ;       // Taille attribut `data` de la scene 
    int end ;       // Trigger de la fin du jeu, prends valeur TRUE ou FALSE 
    uint32_t startTime ;
    uint32_t currentTime ;
} InfoScene_t ;

typedef struct SceneEvent_u {
    // fonction qui check si l'event s'active ou se desactive et renvoi l'état de la progression (nb entre 0.0 et 1.0)
    float (*trigger) (Scene_t *, struct SceneEvent_u *) ; 
    void (*action) (Scene_t *, float) ;

    int active ;
} SceneEvent_t ;

typedef struct SceneEventManager_u {
    SceneEvent_t * events ;
    int eventCount ;
    int maxEvent ;
} SceneEventManager_t ;

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

// fonctions SceneEvent 
SceneEventManager_t * create_event_manager(int maxEvent) ;
void destroy_event_manager (SceneEventManager_t ** manager) ;
void destroy_event_manager_cb (void * manager) ;
void init_event_manager (SceneEventManager_t * manager, int maxEvent) ; 
void add_event (SceneEventManager_t * manager, float (*trigger) (Scene_t *, SceneEvent_t *), void (*action) (Scene_t *, float)) ;
void process_events(SceneEventManager_t *manager, Scene_t *scene) ;


// fonctions manip scene level1
#define LEVEL1_NB_EVENT 3
typedef enum LEVEL1_Event_u {L_LOADING, L_BLIS, L_CMD} LEVEL1_Event_t ;
void LEVEL1_load (Scene_t * self) ;
void LEVEL1_unLoad (Scene_t * self) ;
void LEVEL1_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) ;
void LEVEL1_update (Scene_t * self, SceneManager_t * manager) ;
void LEVEL1_render (Scene_t * self) ; 
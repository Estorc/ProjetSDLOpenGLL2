#pragma once 


#include "lib.h"



typedef struct Scene_u Scene_t ;  
typedef struct SceneManager_u SceneManager_t ; 
typedef struct List_u List_t ;

extern SceneManager_t * sceneManager ;

typedef struct Event_u {
    // fonction qui check si l'event s'active ou se desactive et renvoi l'état de la progression (nb entre 0.0 et 1.0)
    float (*trigger) (Scene_t *, struct Event_u *) ; 
    void (*action) (Scene_t *, float) ;

    uint32_t execTime ;
    uint32_t duration ;
    int active ;
} Event_t ;

typedef struct EventManager_u {
    List_t * events ;
    int eventCount ;
} EventManager_t ;


EventManager_t * create_event_manager() ;
void destroy_event_manager (EventManager_t ** manager) ;
void destroy_event_manager_cb (void * manager) ;
void add_event (EventManager_t * manager, uint32_t delay, uint32_t duration, float (*trigger) (Scene_t *, Event_t *), void (*action) (Scene_t *, float)) ;
void process_events(EventManager_t *manager, Scene_t *scene) ;
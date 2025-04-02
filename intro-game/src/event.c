#include "../include/lib.h"
#include "../include/main.h" 
#include "../include/window.h"
#include "../include/scene.h"
#include "../include/list.h"
#include "../include/event.h"


/**
 * 
 */
EventManager_t * create_event_manager() {

    EventManager_t * manager = malloc(sizeof(EventManager_t)) ;
    manager->events = create_list(free_cb) ;
    manager->eventCount = 0;

    return manager ;
}


/**
 * 
 */
void destroy_event_manager (EventManager_t ** manager) {

    if (existe(manager) && existe(*manager)) {

        if (existe((*manager)->events)) {

            destroy_list(&(*manager)->events);
        }

        free(*manager);
        *manager = NULL ;
    }
}
void destroy_event_manager_cb (void * manager) {
    destroy_event_manager(manager);
}


/**
 * 
 */
void add_event (EventManager_t * manager, uint32_t delay, uint32_t duration, 
    float (*trigger) (Scene_t *, Event_t *), void (*action) (Scene_t *, float)) 
{
        
    Event_t * event = malloc(sizeof(Event_t)) ;
    
    event->trigger = trigger ;
    event->action = action ;
    event->execTime = delay + SDL_GetTicks() ;
    event->duration = duration ;
    event->active = TRUE ;
    
    manager->events->set(manager->events, event, manager->events->size);
    manager->eventCount++;
}


/**
 * 
 */
void process_events(EventManager_t * manager, Scene_t * scene) {

    for (int i = 0; i < manager->eventCount; i++) {

        Event_t * event = manager->events->item(manager->events, i) ;

        float progress = event->trigger(scene, event) ;

        if (progress >= 1.0f) {
            progress = 1.0f;     // On s'assure que progress ne dépasse pas 1
            event->action(scene, progress);
        
            manager->events->remove(manager->events, i);  // L'événement est terminé, on le retire de la liste
            manager->eventCount--;
        }
        else if (progress > 0) {
            event->action(scene, progress);
        }
    }
}
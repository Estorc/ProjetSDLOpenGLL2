#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/render.h"
#include "../include/scene.h"
#include "../include/text.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"
#include "../include/list.h" 
#include "../include/event.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/camera.h"


void LEVEL1_load (Scene_t * self) {
    
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return ;
    }

    self->data = create_dictionnary() ;
    if (!existe(self->data)) {
        fprintf(stderr, "Erreur création dictionnaire dans DESKTOP\n");
        return ;
    }

    Dictionary_t * dict = self->data ;

    // Allocation et initialisation du compteur d'éléments
    InfoScene_t * info = malloc(sizeof(InfoScene_t));
    if (info == NULL) {
        fprintf(stderr, "Erreur: allocation mémoire échouée pour `len`\n");
        destroy_dictionary(&self->data);
        return ;
    }
    info->len = 4;
    info->end = FALSE ;


    Player_t * player = player_constructor();
    if (player == NULL) {
        fprintf(stderr, "Erreur creation player\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "player", player, player_destructor_cb);


    Map_t * map = map_constructor();
    if (map == NULL) {
        fprintf(stderr, "Erreur creation map\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "map", map, map_destructor_cb);


    Camera_t * camera = camera_constructor (player);
    if (camera == NULL) {
        fprintf(stderr, "Erreur creation camera\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "camera", camera, camera_destructor_cb);


    EventManager_t * eventManager = create_event_manager() ;
    if (!existe(eventManager)) {
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "eventManager", eventManager, destroy_event_manager_cb);


    // Stockage dans `self->data`
    dict->set(dict, "info", info, free_cb);

    printf("[INFO] : Chargement des données LEVEL1 réussi\n");
    
    return ; 
} 


void LEVEL1_unLoad (Scene_t * self) {
    
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return;
    }

    if (self->data == NULL) {
        fprintf(stderr, "Aucune donnée à libérer dans `self->data`\n");
        return;
    } 
    
    destroy_dictionary(&self->data);

    printf("[INFO] : Déchargement des données LEVEL1 réussi\n");
    
    return ; 
}


void LEVEL1_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) {
    
    InfoScene_t * info = GET_INFO(self->data) ;
    Player_t * player = GET_PLAYER(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    uint32_t buttonClicked = FALSE;
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT : 
                info->end = TRUE ;
                break;
            case SDL_KEYDOWN :
                switch (event->key.keysym.sym) {
                    case SDLK_q :
                        break;
                    case SDLK_d :
                        break;
                    case SDLK_SPACE :
                        break;
                    case SDLK_BACKSPACE : 
                        info->end = TRUE ;
                        break;
                    case SDLK_f :
                        camera->followPlayer = (camera->followPlayer + 1) % 2; printf("changement follow\n") ;
                        break;
                    case SDLK_i :
                        printf("INFO :\nplayer x, y, vx, vy : %f %f %f %f\ncamera x, y : %f %f\n", player->body.position.x, player->body.position.y, player->body.vx, player->body.vy, camera->x, camera->y);
                        break;
                    default :
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                buttonClicked = SDL_MOUSEBUTTONDOWN;
                break;
            case SDL_MOUSEBUTTONUP :
                buttonClicked = SDL_MOUSEBUTTONUP;
                break;
        }
    }


    const uint8_t * keys = SDL_GetKeyboardState(NULL);
    handle_input(keys, player);

    
    return ; 
}


void LEVEL1_update (Scene_t * self, SceneManager_t * manager) {

    Player_t * player = GET_PLAYER(self->data) ;
    Map_t * map = GET_MAP(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    // met a jour la position des objets dynamique 
    update_player(player, &map->ground);
    update_camera(camera, player);

    gameStatus.updateCount++;

    if (gameStatus.updateCount == 5) {
        gameStatus.updateCount = 0;
        update_player_anim_state (player);
    }    

    return ; 
}


void LEVEL1_render (Scene_t * self) {

    Player_t * player = GET_PLAYER(self->data) ;
    Map_t * map = GET_MAP(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    // dessine le rendu 
    if (draw (camera, player, map)) {
        gameStatus.running = 0;
    }

}
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


#define FLAME_BALL 0 


// ensemble des evenements de la scene
static float new_projectile_trigger (Scene_t * scene, Event_t * event) ;
static void new_projectile (Scene_t * scene, float progress) ;


static void init_flame_ball_vertical (MapObj_t * object, List_t * listSpriteSheet, size_t idSpriteSheet) ;
static void init_map (Map_t * map) ;


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
    init_map(map);
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


    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
    process_events(eventManager, self);

    
    return ; 
}


void LEVEL1_update (Scene_t * self, SceneManager_t * manager) {

    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;

    Player_t * player = GET_PLAYER(self->data) ;
    Map_t * map = GET_MAP(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    // met a jour la position des objets dynamique 
    update_player(player, &map->ground);
    update_camera(camera, player);
    map_update(self, map, player);

    if ( (rand() % 100) == 0 ) {
        add_event(eventManager, 0, 0, new_projectile_trigger, new_projectile);
    }

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


static 
float new_projectile_trigger (Scene_t * scene, Event_t * event) {
    return 1.0f ;
}
static
void new_projectile (Scene_t * scene, float progress) {
    
    Map_t * map = GET_MAP(scene->data) ;

    MapObj_t * object = create_mapObj(map->listSpriteSheet, FLAME_BALL) ;
    if (object == NULL) {
        printf("Impossible d'ajouter une FLAME BALL, erreur creation object\n");
        return ;
    }

    init_flame_ball_vertical(object, map->listSpriteSheet, FLAME_BALL) ;
    map->listObjects->stack(map->listObjects, object);

    printf("creation d'un obj a la position x = %d, y = %d\n", object->sprite.position.x, object->sprite.position.y);
}


/**
 * Initialise une flame ball vertical dans une position aléatoire (sur l'axis X) en haut de la map.  
 */
static
void init_flame_ball_vertical (MapObj_t * object, List_t * listSpriteSheet, size_t idSpriteSheet) {
    
    if (!existe(object)) {
        printf("impossible d'init l'object car NULL\n");
        return ;
    }

    object->sprite.texture = listSpriteSheet->item(listSpriteSheet, idSpriteSheet) ;
    object->sprite.animationSpeed = 5 ;
    object->sprite.currentFrame = 0 ;
    object->sprite.frameCount = 0 ;
    object->sprite.hidden = FALSE ;
    object->sprite.loop = TRUE ;
    object->sprite.numFrames = 6 ;
    object->sprite.playing = TRUE ;
    object->sprite.position = (SDL_Rect){ (BACKGROUND_WIDTH / 20) * (rand() % 20) , 0 , 25 , 189 } ;
    object->sprite.srcrect = (SDL_Rect){0, 0, 25, 189} ;
    object->sprite.typeAnim = DEFAULT ;

    object->vx = 0 ;
    object->vy = 1 ;

    object->idAction = ACT_ID_LOSS_PV ;
    object->idSpriteSheet = idSpriteSheet ;
}


static
void init_map (Map_t * map) {

    if (existe(map)) {

        map->background = load_png("intro-game/assets/background_grey.png");
        if (map->background == NULL) {
            printf("Erreur background dans init map level1\n");
            return ;
        }

        map->ground = (SDL_FRect){0, BACKGROUND_HEIGHT - GROUND_HEIGHT, GROUND_WIDTH, GROUND_HEIGHT} ;

        SDL_Texture * spriteSheet1 = load_png("intro-game/assets/projectileVertical.png") ;
        if (spriteSheet1 == NULL) {
            printf("Erreur spriteSheet dans init map level1\n");
            return ;
        }
        map->listSpriteSheet->stack(map->listSpriteSheet, spriteSheet1);
    }
}
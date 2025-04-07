#include "../include/lib.h"
#include "../include/main.h"
#include "../include/map.h"
#include "../include/texture_loader.h"
#include "../include/list.h"
#include "../include/player.h"
#include "../include/scene.h"

 
Map_t * map_constructor () {

    Map_t * map = malloc(sizeof(Map_t));
    if (map == NULL) {
        printf("Erreur malloc map : %s\n", SDL_GetError());
        return NULL;
    }
 
    // charge l'image de fond
    map->background = NULL ;

    map->listSpriteSheet = create_list(SDL_DestroyTexture_cb) ;
    map->listObjects = create_list(destroy_mapObj_cb) ;


    return map;
}

void map_destructor (Map_t ** map) {
    
    if (map != NULL && existe(map)) {
        
        SDL_DestroyTexture((*map)->background);

        destroy_list(&(*map)->listSpriteSheet) ;
        destroy_list(&(*map)->listObjects) ;

        free(*map);
        *map = NULL;
    }
}
void map_destructor_cb (void * map) {
    map_destructor(map);
}


MapObj_t * create_mapObj () {

    MapObj_t * object = malloc(sizeof(MapObj_t)) ;
    if (!existe(object)) {
        fprintf(stderr, "Erreur malloc de MapObj_t\n");
        return NULL ;
    }

    return object ;
}


void destroy_mapObj (MapObj_t ** object) {

    if (object != NULL && existe(*object)) {

        free(*object);
        *object = NULL ;
    }
}
void destroy_mapObj_cb (void * object) {
    destroy_mapObj(object);
}


void map_update (Scene_t * scene, Map_t * map, Player_t * player) {

    if (scene == NULL || map == NULL || player == NULL) {
        printf("Impossible d'update la map, un des parametres est NULL\n");
        return ;
    }


    // met a jour la position et l'animation de chaque objet
    for (int i = 0; i < map->listObjects->size; i++) {

        MapObj_t * object = map->listObjects->item(map->listObjects, i) ;


        if (object->sprite.hidden == FALSE) {
            
            texture_update(&object->sprite);

            object->sprite.position.x += object->vx ;
            object->sprite.position.y += object->vy ;

            SDL_Rect playerBody = (SDL_Rect){player->body.position.x, player->body.position.y, player->body.position.w, player->body.position.h} ;
            if (SDL_Rect_check_collision(&playerBody, &object->sprite.position)) {
                mapObj_action(scene, map, player, object->idAction);
                map->listObjects->remove(map->listObjects, i);
            }

            SDL_Rect groundRect = {map->ground.x, map->ground.y, map->ground.w, map->ground.h} ;
            if (SDL_Rect_check_collision(&groundRect, &object->sprite.position)) {
                map->listObjects->remove(map->listObjects, i);
                printf("destruction d'un obj\n");
            }
        }
    }
}


void mapObj_action (Scene_t * scene, Map_t * map, Player_t * player, uint8_t idAction) {

    switch (idAction) {
    
    case ACT_ID_LOSS_PV :
        printf("action 0 effectuee sur map\n");
        if (player->pv >= 0)
            player->pv--;
        break;
    
    default:
        break;
    }
}
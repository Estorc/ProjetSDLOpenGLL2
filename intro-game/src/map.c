#include "../include/lib.h"
#include "../include/main.h"
#include "../include/map.h"
#include "../include/texture_loader.h"


Map_t * map_constructor () {

    Map_t * map = malloc(sizeof(Map_t));
    if (map == NULL) {
        printf("Erreur malloc map : %s\n", SDL_GetError());
        return NULL;
    }
    
    // donne un zone de cillison pour le sol 
    SDL_FRect ground = {
        .x = 0,
        .y = BACKGROUND_HEIGHT - GROUND_HEIGHT,
        .w = GROUND_WIDTH,
        .h = GROUND_HEIGHT
    };
    map->ground = ground;
 
    // charge l'image de fond
    map->background = load_png("intro-game/assets/background_grey.png");
    if (map->background == NULL) {
        return NULL;
    }


    return map;
}

void map_destructor (Map_t ** map) {
    SDL_DestroyTexture((*map)->background);
    
    free(*map);
    *map = NULL;
}
void map_destructor_cb (void * map) {
    map_destructor(map);
}
#include "../include/lib.h"
#include "../include/main.h"
#include "../include/player.h"
#include "../include/camera.h"
#include "../include/map.h"


Camera_t * camera_constructor (Player_t * player) {
    Camera_t * cam = malloc (sizeof(Camera_t));
    if (cam == NULL) {
        printf("Erreur malloc camera : %s\n", SDL_GetError());
        return NULL;
    }

    cam->x = 0;
    cam->y = 0;
    cam->width = CAMERA_WIDTH;
    cam->height = CAMERA_HEIGHT;
    cam->deadzone = player->body.position.w * 1.5;
    cam->followPlayer = FALSE;

    return cam;
}


void camera_destructor (Camera_t ** camera) {
    free(*camera);
    *camera = NULL;
}


/**
 * La position de la camera est placée de sorte a avoir le joueur en plein milieu
 * Elle est repositionnée pour ne pas sortir de la map si besoin 
 */
void update_camera (Camera_t * camera, Player_t * player) {
    camera->x = player->body.position.x + (PLAYER_WIDTH / 2) - (CAMERA_WIDTH / 2); 

    // en fonction du suivi du joueur 
    if (camera->followPlayer)
        camera->y = player->body.position.y + (PLAYER_HEIGHT / 2) - (CAMERA_HEIGHT / 2);
    else 
        camera->y = BACKGROUND_HEIGHT - CAMERA_HEIGHT;
    
    // repositionne x, si x depasse des limites de la map
    if (camera->x < 0) camera->x = 0;
    if ((camera->x + CAMERA_WIDTH) > BACKGROUND_WIDTH) camera->x = BACKGROUND_WIDTH - CAMERA_WIDTH;
    
    // repositionne y, si y depasse des limites de la map
    if (camera->y < 0) camera->y = 0;
    if ((camera->y + CAMERA_HEIGHT) > BACKGROUND_HEIGHT) camera->y = BACKGROUND_HEIGHT - CAMERA_HEIGHT;
}


/**
 * La position de la camera est placer aleatoirement entre les 4 coins de la fenetre : 
 * en haut a gauche, en haut a droite, en bas a gauche ou en bas a droite
 */
void update_camera_random (Camera_t * camera) {
    camera->x = WINDOW_WIDTH * (rand() % 2);
    camera->y = WINDOW_HEIGHT * (rand() % 2);
}
#pragma once 

#include "window.h"
#include "player.h"

#define CAMERA_WIDTH 1080 
#define CAMERA_HEIGHT 720

typedef struct Camera_u {
    float x, y;
    float width, height;
    float deadzone;
    int followPlayer;
} Camera_t ;

Camera_t * camera_constructor (Player_t * player);
void camera_destructor (Camera_t ** camera);
void camera_destructor_cb (void * camera) ;
void update_camera (Camera_t * camera, Player_t * player);
void update_camera_random (Camera_t * camera);
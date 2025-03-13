#pragma once 

#include "commun.h"

#define TRUE 1
#define FALSE 0

#define NO_ERR 0
#define ERROR 1

#define FPS 60
#define FRAME_DELAY (1000 / FPS)

typedef struct Camera_u Camera_t ;
typedef struct Player_u Player_t ;
typedef struct Map_u Map_t ;
typedef int err_t ;

typedef struct {
    int running ; 
    int frameCount ;
    int updateCount ;
    int scene ;

    int (*(*playScene)) (Camera_t *, Player_t *, Map_t *);
} GameStatus_t ; 


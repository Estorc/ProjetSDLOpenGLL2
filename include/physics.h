#pragma once 

#include "lib.h"

typedef struct RigidBody_u {
    SDL_FRect position;
    SDL_Texture * texture;
    float vx, vy;           // velocité 
    float jump;             // valeur de saut 
    int onGround;           // si au sol  
} RigidBody_t ;

typedef struct StaticBody_u {
    SDL_FRect rec; 
    SDL_Texture * texture;
} StaticBody_t ;


void update_position (RigidBody_t * body);
int check_collision (SDL_FRect * a, SDL_FRect * b);
int SDL_Rect_check_collision(SDL_Rect * a, SDL_Rect * b);
void ground_collision (RigidBody_t * object, SDL_FRect * ground);
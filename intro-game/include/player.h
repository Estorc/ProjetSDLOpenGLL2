#pragma once 


#include "physics.h"
#include "window.h"

extern GameStatus_t gameStatus ;


#define PLAYER_WIDTH 150 
#define PLAYER_HEIGHT 150  

#define PLAYER_SPRITE_WIDTH 128
#define PLAYER_SPRITE_HEIGHT 80

#define JUMP_FORCE (PLAYER_HEIGHT * 2)

#define NB_IDLE_ANIM 7
#define NB_RUN_ANIM 10
#define NB_JUMP_ANIM 10
#define NB_WALK_ANIM 10

typedef enum {IDLE, JUMP, RUN, WALK} ActionState_t ;

typedef struct Player_u {
    RigidBody_t body ;
    
    float acceleration, deceleration ;
    float vMax ;

    ActionState_t actionState ;
    int animState ;
} Player_t ;


Player_t * player_constructor ();
void player_destructor (Player_t ** player);
void player_destructor_cb (void * player) ;
void update_player (Player_t * player, SDL_FRect * ground);
void handle_input (const uint8_t * keys, Player_t * player);
void update_player_anim_state (Player_t * player);
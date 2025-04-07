#include "../include/lib.h"
#include "../include/main.h"
#include "../include/player.h"
#include "../include/texture_loader.h"
#include "../include/camera.h"
#include "../include/map.h"


Player_t * player_constructor () {
    Player_t * player = malloc(sizeof(Player_t));
    if (player == NULL) {
        printf("Erreur malloc player : %s\n", SDL_GetError());
        return NULL;
    }

    player->body.position.w = PLAYER_WIDTH;
    player->body.position.h = PLAYER_HEIGHT;
    player->body.position.x = WINDOW_WIDTH / 2;
    player->body.position.y = WINDOW_HEIGHT / 2 - 200;
    player->body.vx = 0;
    player->body.vy = 0;
    player->body.jump = 0; 
    player->body.onGround = FALSE;

    player->acceleration = 1; 
    player->deceleration = 0.3;
    player->vMax = 10;

    player->body.texture = load_png("intro-game/assets/playerSpriteSheet_grey.png");
    if (player->body.texture == NULL) {
        return NULL;
    }

    player->actionState = IDLE;
    player->animState = 0;

    return player;
}


void player_destructor (Player_t ** player) {
    SDL_DestroyTexture((*player)->body.texture);
    free(*player);
    *player = NULL;
}
void player_destructor_cb (void * player) {
    player_destructor(player);
}


/* Incremente anim state pour afficher le sprite suivant de l'animation 
 * N'effectue pas de changement lorsque le player est en phase de saut car cette fonction est basée sur le temps
 * or le saut change de frame d'animation en fonction de sa position dans les airs non pas le temps passée.
 * Le changement d'anim_state durant le saut est traité par la fonction draw_player().
 */ 
void update_player_anim_state (Player_t * player) {
    switch (player->actionState) {
    case IDLE :
        player->animState = (player->animState + 1) % NB_IDLE_ANIM;
        break;
    case RUN :
        player->animState = (player->animState + 1) % NB_RUN_ANIM;
        break;
    case WALK :
        player->animState = (player->animState + 1) % NB_WALK_ANIM;
    default:
        break;
    }
}


void update_player (Player_t * player, SDL_FRect * ground) {
    // gere la hauteur du personnage en fonction du saut
    if (!player->body.onGround) {
        if (player->body.jump > (JUMP_FORCE / 2)) {
            player->body.jump -= 5;
            player->body.vy = -5;
        }
        else if (player->body.jump > 0) {
            player->body.jump -= 5;
            player->body.vy = 7;
        }
        else {
            player->body.jump = 0;
            player->body.vy = 5;
        }
    }

    // gere la position x 
    update_position(&player->body);
    
    // replace le player pour ne pas sortir de la map 
    if (player->body.position.x < 0) player->body.position.x = 0;
    if ((player->body.position.x + player->body.position.w) > BACKGROUND_WIDTH) player->body.position.x = BACKGROUND_WIDTH - player->body.position.w;

    ground_collision(&player->body, ground); 
}


void handle_input (const uint8_t * keys, Player_t * player) {
    if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
        // if (player->body.vx < 0 && (player->body.vx == player->vMax)) 
        //     player->body.vx = -player->body.vx;

        player->body.vx += player->acceleration ;
        if (player->body.vx > player->vMax) 
            player->body.vx = player->vMax;
    } 
    else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
        // if (player->body.vx > 0 && (player->body.vx == player->vMax)) 
        //     player->body.vx = -player->body.vx;
        
        player->body.vx -= player->acceleration ;
        if (player->body.vx < -player->vMax) 
            player->body.vx = -player->vMax;
    }
    else if (player->body.vx != 0) {
        if (player->body.vx > 0) {
            player->body.vx -= player->deceleration ;
            if (player->body.vx < 0) 
                player->body.vx = 0;
        }
        else if (player->body.vx < 0) {
            player->body.vx += player->deceleration ;
            if (player->body.vx > 0) 
                player->body.vx = 0;
        }
    }
    if (keys[SDL_SCANCODE_SPACE] && player->body.onGround == TRUE) {
        player->body.jump = JUMP_FORCE;
        player->body.onGround = FALSE;
    }
}
#include "../include/lib.h"
#include "../include/main.h"
#include "../include/physics.h"


// met a jour a position x et y en focntion de la velocite 
void update_position (RigidBody_t * body) {
    body->position.x += body->vx ; 
    body->position.y += body->vy ; 
}

// renvoi 1 si a en colision avec b, 0 sinon
int check_collision(SDL_FRect * a, SDL_FRect * b) {
    return (a->x < b->x + b->w &&
            a->x + a->w > b->x &&
            a->y < b->y + b->h &&
            a->y + a->h > b->y);
}

int SDL_Rect_check_collision(SDL_Rect * a, SDL_Rect * b) {
    return (a->x < b->x + b->w &&
            a->x + a->w > b->x &&
            a->y < b->y + b->h &&
            a->y + a->h > b->y);
}

// si l'objet est en collision avec le sol, met a jour ses coordonnes 
void ground_collision (RigidBody_t * object, SDL_FRect * ground) {
    if (check_collision(&object->position, ground)) {
        object->position.y = ground->y - object->position.w;
        object->vy = 0;
        object->onGround = TRUE;
        object->jump = 0;
    }
}
#include "../include/lib.h"
#include "../include/main.h"
#include "../include/texture_loader.h" 
#include "../include/map.h"
#include "../include/camera.h" 
#include "../include/player.h"
#include "../include/render.h"
#include "../include/text.h"


/**
 * 
 */
err_t draw_text (Text_t * text) {

    SDL_Rect posHollow = {text->animation.position.x - 3, text->animation.position.y + 3, text->animation.position.w, text->animation.position.h} ;

    if (text->animation.hollow == TRUE && existe(text->animation.hollowTexture)) {
        SDL_RenderCopy(renderer, text->animation.hollowTexture, NULL, &posHollow);
    }
    
    if (existe(text->animation.texture)) {
        SDL_RenderCopy(renderer, text->animation.texture, NULL, &text->animation.position);
    }

    return NO_ERR ;
} 


int mapPrevX, mapPrevY;
/**
 * 
 */
void draw_map (Map_t * map, Camera_t * camera) {
    // methdoe decoupage de la tecture puis affichage sur la fenetre complete 
    SDL_Rect srcrect = {camera->x, camera->y, camera->width, camera->height};
    if ((srcrect.x + srcrect.w) > BACKGROUND_WIDTH) srcrect.x = BACKGROUND_WIDTH - srcrect.w;
    SDL_RenderCopy(renderer, map->background, &srcrect, NULL);

    // methode affichage complet de la map sur une position bien precise 
    // SDL_FRect dstrect = {-camera->x, -camera->y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};
    // SDL_RenderCopyF(renderer, map->background, NULL, &dstrect);

    // if ((mapPrevX - srcrect.x) != 0) {
    //     printf("map new coordinates x : %d, y : %d\n", mapPrevX - srcrect.x, mapPrevY - srcrect.y);
    //     mapPrevX = srcrect.x;
    //     mapPrevY = srcrect.y;
    // }

    // dessine la zone de collision du sol 
    // SDL_Rect dstrect = {0, map->ground.rec.y, WINDOW_WIDTH, map->ground.rec.height}; 
    
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderFillRect(renderer, &dstrect);
}


void draw_background (Map_t * map, Camera_t * camera) {
    // defini la partie de la texture a afficher
    SDL_Rect srcrect = {camera->x, camera->y, camera->width, camera->height};
    if ((srcrect.x + srcrect.w) > BACKGROUND_WIDTH) srcrect.x = BACKGROUND_WIDTH - srcrect.w;
    
    // affiche la texture du background sur toute la fenetre
    SDL_RenderCopy(renderer, map->background, &srcrect, NULL);
}


/**
 * Pour afficher les elements relativement a la fenetre et non pas a la camera, passée NULL a camera.
 */
void draw_desktop (Desktop_t * desktop) {

    SDL_RenderCopy(renderer, desktop->background, NULL, NULL);

    for (int i = 0; i < desktop->nbElem; i++) {
        // affiche l'element si il est visible
        if (desktop->elements[i].hidden == FALSE) {
            SDL_RenderCopy(renderer, desktop->elementSpritesheet, &desktop->elements[i].srcrect, &desktop->elements[i].position);

            if (desktop->elements[i].clicked == TRUE) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 231, 244, 244, 100);

                SDL_RenderFillRect(renderer, &desktop->elements[i].position);
            }
        }
    }
}


static SDL_RendererFlip flip ;
void draw_player (Player_t * player, Camera_t * camera) {

    // definie l'action que fait le personnage (courir, marcher, sur place, sauter)
    if (player->body.onGround == FALSE) {
        // definie l'anim state pour connaitre le sprite d'animation de saut a affiché
        player->animState = player->body.jump > 0 ? (JUMP_FORCE - player->body.jump) / (JUMP_FORCE / NB_JUMP_ANIM) : NB_JUMP_ANIM - 1 ;
        player->actionState = JUMP;

        if (player->body.vx < 0) flip = SDL_FLIP_HORIZONTAL;
        else flip = SDL_FLIP_NONE ;
    }
    else if (player->body.vx > 0) {
        flip = SDL_FLIP_NONE;
        if (player->body.vx == player->vMax)
            player->actionState = RUN;
        else 
            player->actionState = WALK;
    }
    else if (player->body.vx < 0) {
        flip = SDL_FLIP_HORIZONTAL;
        if (player->body.vx == -player->vMax)
            player->actionState = RUN;
        else 
            player->actionState = WALK;
    }
    else {
        player->actionState = IDLE;
        if (player->animState >= NB_IDLE_ANIM) 
            player->animState = 0;
    }

    SDL_FRect position = player->body.position;
    SDL_Rect dstrect = {position.x - camera->x, position.y - camera->y, position.w, position.h};
    SDL_Rect srcrect = {PLAYER_SPRITE_WIDTH * player->animState + 20, PLAYER_SPRITE_HEIGHT * player->actionState, PLAYER_SPRITE_HEIGHT, PLAYER_SPRITE_HEIGHT};

    SDL_RenderCopyEx(renderer, player->body.texture, &srcrect, &dstrect, 0, NULL, flip);

    // dessine la zone de collision du player 
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_RenderDrawRect(renderer, &dstrect);
}


static SDL_Texture * noiseTexture ;
int generate_noise_texture(int width, int height) {
    
    // creer surface
    SDL_Surface * surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (surface == NULL) {
        printf("Erreur generate surface for noise texture : %s\n", SDL_GetError());
        return 1 ;
    }


    // change la valeur des pixels 
    Uint32 *pixels = (Uint32 *)surface->pixels;
    for (int i = 0; i < width * height; i++) {
        if (rand() % 10 > 4) {
            Uint8 noise = rand() % 256;
            pixels[i] = SDL_MapRGBA(surface->format, noise, noise, noise, 100); // Alpha faible
        }
    }

    // creer texture a partir de la surface 
    noiseTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (noiseTexture == NULL) {
        printf("Erreur generate texture for noise texture : %s\n", SDL_GetError());
        return 1 ;
    }
    
    // libere les surface
    SDL_FreeSurface(surface);

    return 0;
}
void destroy_noise_texture () {
    SDL_DestroyTexture(noiseTexture);
}


static SDL_Texture * desktopGlitchTexture ;
/**
 * Si erreur lors du chargement retourne 1 sinon retourne 0
 */
int generate_desktop_glitch_texture () {
    desktopGlitchTexture = load_png("intro-game/assets/desktop_glitch.png");
    if (desktopGlitchTexture == NULL) {
        return 1;
    }
    return 0;
}
void destroy_desktop_glitch_texture () {
    SDL_DestroyTexture(desktopGlitchTexture);
}


void apply_glitch(Camera_t * camera, SDL_Texture *texture, int width, int height) {
    
    int axis = rand() % 2 ; // 0 pour horizontal, 1 pour vertical

    float widthMax = WINDOW_WIDTH / 4, widthMin = WINDOW_WIDTH / 8;
    float heightMax = WINDOW_HEIGHT / 4, heightMin = WINDOW_HEIGHT / 8;
    int gap = 10;
    int maxRender = 40;
    for (int i = 0; i < maxRender; i++) {
        if (axis == 0) {        // decalage pixel a l'horizontal 
            float width = rand() % (int) (widthMax - widthMin) + widthMin ;
            float height = gap ;
            float x = rand() % (int) (WINDOW_WIDTH - width) ;
            float y = height * (rand() % (int) ((WINDOW_HEIGHT / height) - 2) + 1) ;

            gap = (rand() % 2 ? gap : -gap) ;      // distance decalage de la ranger de pixel  

            SDL_Rect srcrect = {x + camera->x, y + camera->y, width, height};
            SDL_Rect dstrect = {x, y + gap, width, height};
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        }
        else {
            float width = gap ;
            float height = rand() % (int) (heightMax - heightMin) + heightMin ;
            float x = width * (rand() % (int) ((WINDOW_WIDTH / width) - 2) + 1) ;
            float y = rand() % (int) (WINDOW_HEIGHT - height) ;

            gap = (rand() % 2 ? gap : -gap) ;      // distance decalage de la ranger de pixel  

            SDL_Rect srcrect = {x + camera->x, y + camera->y, width, height};
            SDL_Rect dstrect = {x + gap, y, width, height};
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        }
    }

    
}


void draw_scene0 (Camera_t * camera, Map_t * map) {
    // if (rand() % 10 > 8) 
    //     draw_background(map, camera);
    
    // draw_listElements(map, NULL);
}


// affiche l'ecran 
// retourne 1 en cas d'erreu ou 0 si aucune erreur 
int draw (Camera_t * camera, Player_t * player, Map_t * map) {
    
    switch (gameStatus.scene) {
        case 0 : 
            draw_scene0(camera, map);
            break;
        case 1 : 
            draw_map(map, camera);
            draw_player(player, camera);
            // if (rand() % 10 > 6)
            //     apply_glitch(camera, map->background, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
            break;
        default : 
            break;
    }

    return 0;
}
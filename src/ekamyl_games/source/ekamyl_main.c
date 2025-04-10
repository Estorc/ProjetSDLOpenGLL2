#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/camera.h"
#include "../include/render.h"
#include "../include/gui.h"
#include "../include/scene.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"

static SDL_Window * window ; 
SDL_Renderer * renderer ; 
SceneManager_t * sceneManager ;
SDL_Texture * render_texture ;
GameStatus_t gameStatus ; 


// initialise les variables globales du systeme
int init_systeme () {

    // Création de la fenêtre
    window = SDL_CreateWindow("Intro Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetWindowResizable(window, SDL_FALSE);


    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return 1;
    }

    render_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT) ;
    SDL_SetRenderTarget(renderer, render_texture);
    
    gameStatus.running = TRUE;
    gameStatus.scene = 0;
    gameStatus.frameCount = 0;
    gameStatus.updateCount = 0;

    // initialise le scene manager  
    sceneManager = create_scene_manager() ;
    Scene_t * bootScene = create_scene("BOOT", BOOT_load, BOOT_unLoad, BOOT_handleEvents, BOOT_update, BOOT_render);
    Scene_t * desktopScene = create_scene("DESKTOP", DESKTOP_load, DESKTOP_unLoad, DESKTOP_handleEvents, DESKTOP_update, DESKTOP_render);
    Scene_t * level1Scene = create_scene("LEVEL1", LEVEL1_load, LEVEL1_unLoad, LEVEL1_handleEvents, LEVEL1_update, LEVEL1_render);
    push_scene(sceneManager, bootScene);
    push_scene(sceneManager, desktopScene);
    push_scene(sceneManager, level1Scene);
    request_scene_change(sceneManager, "BOOT");
    change_scene(sceneManager);

    return 0;
}


int ekamyl_play() {

    uint32_t timerStart; 
    
    // variable gestion evenements 
    SDL_Event event ;
    int running = TRUE;

    *timerStart = SDL_GetTicks();

    running = play_scene(sceneManager, &event) ; 

    uint32_t timerDelay = SDL_GetTicks() - *timerStart;
    if (timerDelay < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - timerDelay);
    }


    return running ;
}


void terminate_system () {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(render_texture);
    destroy_scene_manager(&sceneManager);

}
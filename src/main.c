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


int init_systeme ();
void terminate_system (Mix_Chunk * music, int audio, int ttf, int mixer, int img, int sdl);
void print_fps (uint32_t * previousTime);
void start_frame (uint32_t * timerStart);
void end_frame (uint32_t * timerStart, uint32_t * previousTime);

SDL_Window * window ; 
SDL_Renderer * renderer ; 
SceneManager_t * sceneManager ;
SDL_Texture * render_texture ;
GameStatus_t gameStatus ; 

int main(int argc, char* argv[]) {

    if (init_systeme()) {
        return 1;
    }
    
    
    // variable pour l'affichage du nombre de FPS 
    uint32_t previousTime = SDL_GetTicks(); // to print fps every second 
    uint32_t timerStart; 
    
    // variable gestion evenements 
    SDL_Event event ;
    int running = TRUE;

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
    
    // Boucle principale
    while (running) { 

        // traitement debut frame 
        start_frame(&timerStart);

        // Joue scene et vérification de l'état du jeu (fin ou continuer)
        if (play_scene(sceneManager, &event)) { 
            running = FALSE ;
        } 
        
        // traitement fin frame  
        end_frame(&timerStart, &previousTime);
    }
    
    // Nettoyage
    destroy_scene_manager(&sceneManager);
    terminate_system(NULL, TRUE, TRUE, TRUE, TRUE, TRUE);

    printf("fin propre\n");

    return 0;
}


void start_frame (uint32_t * timerStart) {
    *timerStart = SDL_GetTicks();
}


void end_frame (uint32_t * timerStart, uint32_t * previousTime) {

    gameStatus.frameCount++;

    uint32_t timerDelay = SDL_GetTicks() - *timerStart;
    if (timerDelay < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - timerDelay);
    }
}


void print_fps (uint32_t * previousTime) {
    uint32_t time = SDL_GetTicks();
    if (time - *previousTime > 1000) {
        *previousTime = time;
        printf("FPS : %d\n", gameStatus.frameCount);
        gameStatus.frameCount = 0;
    }
}


// initialise les variables globales du systeme et les differentes librairies 
int init_systeme () {

    srand(time(NULL));

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }


    // Initialisation de SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Erreur d'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }


    // initialisation de SDL_ttf 
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }


    // Initialisation de SDL_mixer et configuration audio 
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG )) {
        printf("Erreur d'initialisation de SDL_mixer : %s\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur d'initialisation de Mix_OpenAudio : %s\n", Mix_GetError());
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }


    // Création de la fenêtre
    window = SDL_CreateWindow("Intro Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowResizable(window, SDL_FALSE);


    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    render_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT) ;
    
    gameStatus.running = TRUE;
    gameStatus.scene = 0;
    gameStatus.frameCount = 0;
    gameStatus.updateCount = 0;

    return 0;
}


void terminate_system (Mix_Chunk * music, int audio, int ttf, int mixer, int img, int sdl) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (music) {
        Mix_FreeChunk(music);
    }
    if (audio) {
        Mix_CloseAudio();
    }
    if (ttf) {
        TTF_Quit();
    }
    if (mixer) {
        Mix_Quit();
    }
    if (img) {
        IMG_Quit();
    }
    if (sdl) {
        SDL_Quit();
    }

}
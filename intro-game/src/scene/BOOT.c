#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/render.h"
#include "../include/scene.h"
#include "../include/text.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"
#include "../include/list.h" 
#include "../include/event.h"
#include "../include/camera.h"


#define STATE_LOADING 0
#define STATE_BLIS 1

#define I_LOADING_WHEEL 0
#define I_BLIS_FL5 1

// fonction de changelment d'etat 
static void BOOT_change_state (Scene_t * self, InfoScene_t * info) ;

// scripts evenements 
static float imageFadeIn_trigger (Scene_t * scene, Event_t * event) ;
static void imageFadeIn (Scene_t * scene, float progress) ;

static float wait_to_loading_trigger (Scene_t * scene, Event_t * event) ;
static void wait_to_loading (Scene_t * scene, float progress) ;

static float loading_bar_trigger (Scene_t * scene, Event_t * event) ;
static void loading_bar (Scene_t * scene, float progress) ;

static float to_next_scene_trigger (Scene_t * scene, Event_t * event) ;
static void to_next_scene (Scene_t * scene, float progress) ;


/**
 * 
 */
void BOOT_load (Scene_t * self) {
    
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return ;
    }

    self->data = create_dictionnary() ;
    if (!existe(self->data)) {
        return ;
    }

    Dictionary_t * dict = self->data ;

    EventManager_t * eventManager = create_event_manager() ;
    if (!existe(eventManager)) {
        destroy_dictionary(&self->data);
        return ;
    }

    dict->set(dict, "eventManager", eventManager, destroy_event_manager_cb);

    // Allocation et initialisation du compteur d'éléments
    InfoScene_t * info = malloc(sizeof(InfoScene_t));
    if (info == NULL) {
        fprintf(stderr, "Erreur: allocation mémoire échouée pour `len`\n");
        destroy_dictionary(&self->data);
        return ;
    }
    info->len = 3;
    info->end = FALSE ;

    // ajoute info dans la structure data 
    dict->set(dict, "info", info, free_cb);

    // initialise et créé la liste contenant les textures
    List_t * listTexture = create_list(destroy_texture_cb) ;
    if (!existe(listTexture)) {
        fprintf(stderr, "Erreur création dict_textures dans DESKTOP\n");
        destroy_dictionary(&self->data);
        return ;
    }
    
    load_textures_from_file(listTexture, "intro-game/data/donneesTexturesBootScene.csv") ;

    // ajoute la liste des texture dans la data de la scene 
    dict->set(dict, "listTexture", listTexture, destroy_list_cb) ;

    Camera_t * camera = camera_constructor(NULL) ;
    dict->set(dict, "camera", camera, camera_destructor_cb) ;

    // recupere la date de lancement en millisecondes
    info->startTime = SDL_GetTicks() ;

    info->state = STATE_LOADING ;
    info->nextState = STATE_LOADING ;
    BOOT_change_state(self, info) ;

    printf("[INFO] : Chargement des données BOOT réussi\n");
}


/**
 * 
 */
void BOOT_unLoad (Scene_t * self) {
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return;
    }
    
    if (self->data == NULL) {
        fprintf(stderr, "Aucune donnée à libérer dans `self->data`\n");
        return;
    }

    destroy_dictionary(&self->data);

    printf("[INFO] : Déchargement des données BOOT réussi\n");
}


/**
 * 
 */
void BOOT_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) {

    InfoScene_t * info = self->data->get(self->data, "info") ;

    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT :
                info->end = TRUE ;
                break;
            case SDL_KEYDOWN : 
                switch (event->key.keysym.sym) {
                    case SDLK_RETURN : 
                        request_scene_change(sceneManager, "DESKTOP");
                        break;
                    default :
                        break;
                }
            default : 
                break;
        }
    }

    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
    process_events(eventManager, self);
}


/**
 * 
 */
void BOOT_update (Scene_t * self, SceneManager_t * manager) {

    List_t * listTexture = GET_LIST_TEXTURE(self->data) ;
    texture_list_update_from_file(listTexture, "intro-game/data/donneesTexturesBootScene.csv");

    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;

    InfoScene_t * info = GET_INFO(self->data) ;
    switch (info->state) {

    case STATE_LOADING : 
        break;

    case STATE_BLIS : 
        break;

    default : 
        break;
    }


    // effectue le changement d'etat de la scene 
    if (info->nextState != info->state) {
        BOOT_change_state(self, info);   
    }
}


/**
 * 
 */
void BOOT_render (Scene_t * self) {

    List_t * listTexture = GET_LIST_TEXTURE(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    for (int i = 0; i < listTexture->size; i++) {

        Texture_t * texture = listTexture->item(listTexture, i) ;
        draw_texture(texture, camera);
    }

}



static 
void BOOT_change_state (Scene_t * self, InfoScene_t * info) {

    switch (info->nextState) {

    case STATE_LOADING :;
        add_event(GET_EVENT_MANAGER(self->data), 3000, 0, wait_to_loading_trigger, wait_to_loading);

        info->state = info->nextState ;
        break;

    case STATE_BLIS :;
        List_t * listTexture = GET_LIST_TEXTURE(self->data) ;
        Texture_t * loadingWheel = listTexture->item(listTexture, I_LOADING_WHEEL) ;
        
        loadingWheel->hidden = TRUE ;

        add_event(GET_EVENT_MANAGER(self->data), 3000, 1500, imageFadeIn_trigger, imageFadeIn);
        add_event(GET_EVENT_MANAGER(self->data), 10000, 0, to_next_scene_trigger, to_next_scene);

        info->state = info->nextState ;
        break;
    
    default:
        break;
    }
}





static 
float imageFadeIn_trigger (Scene_t * scene, Event_t * event) {

    InfoScene_t * info = GET_INFO(scene->data) ;

    if (info->currentTime >= event->execTime) {

        return (float)(info->currentTime - event->execTime) / (float)(event->duration) ;
    }

    return 0.0f ;
}
static 
void imageFadeIn (Scene_t * scene, float progress) {

    List_t * listTexture = GET_LIST_TEXTURE(scene->data) ; 
    Texture_t * texture = listTexture->item(listTexture, I_BLIS_FL5) ;

    texture->hidden = FALSE ;

    int alpha = (int)(progress * 255.0) ; 
    
    if (existe(texture) && existe(texture->texture)) {
        SDL_SetTextureAlphaMod(texture->texture, alpha);
    }
}

static 
float wait_to_loading_trigger (Scene_t * scene, Event_t * event) {

    InfoScene_t * info = GET_INFO(scene->data) ;

    if (info->currentTime >= event->execTime) {
        return 1.0f ;
    }

    return 0.0f ;
}
static 
void wait_to_loading (Scene_t * scene, float progress) {
    List_t * listTexture = GET_LIST_TEXTURE(scene->data) ;
    Texture_t * loadingWheel = listTexture->item(listTexture, I_LOADING_WHEEL) ;

    loadingWheel->hidden = FALSE ;

    add_event(GET_EVENT_MANAGER(scene->data), 0, 6000, loading_bar_trigger, loading_bar);
}

static 
float loading_bar_trigger (Scene_t * scene, Event_t * event) {
    static uint8_t progress = 0 ;

    InfoScene_t * info = GET_INFO(scene->data) ;

    if (info->currentTime >= event->execTime) {

        if (progress < 100) {
            if (rand() % 10 < 3)
                progress += rand() % 5 ; // ajoute un petit pourcentage a la progression 
        }
        
        if (progress >= 100 && info->currentTime < event->execTime + event->duration) {
            return 99.0 / 100 ;
        }
    }

    return (float)progress / 100 ; 
}
static 
void loading_bar (Scene_t * scene, float progress) {

    List_t * listTexture = GET_LIST_TEXTURE(scene->data) ;
    Texture_t * loadingTexture = listTexture->item(listTexture, I_LOADING_WHEEL);

    int width = 500 ;
    int height = 50 ;
    SDL_Rect rect = {
        .x = loadingTexture->position.x + loadingTexture->position.w / 2 - width / 2,
        .y = loadingTexture->position.y + loadingTexture->position.h + 60,          // Place la barre de chargement 60px en dessous de la roue 
        .h = height,
        .w = (int)(width * progress)          // Longueur en fonction de la progression 
    };

    // Dessine la barre de chargement
    draw_rect_filled((SDL_Rect){rect.x, rect.y, width, rect.h}, (SDL_Color){200, 200, 200, 100}); 
    draw_rect_filled(rect, (SDL_Color){200, 200, 200, 255});

    if (progress >= 1.0f) {
        InfoScene_t * info = GET_INFO(scene->data) ;
        info->nextState = STATE_BLIS;
    }
    
}
static 
float to_next_scene_trigger (Scene_t * scene, Event_t * event) {
    InfoScene_t * info = GET_INFO(scene->data) ;

    if (info->currentTime >= event->execTime) {
        return 1.0f ;
    }

    return 0.0f ;
}
static 
void to_next_scene (Scene_t * scene, float progress) {
    request_scene_change(sceneManager, "DESKTOP");
}

#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/render.h"
#include "../include/scene.h"
#include "../include/text.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"
#include "../include/list.h"


// fonctions correspondant a certains evenements 
static float imageFadeIn_trigger (Scene_t * scene, SceneEvent_t * event) ;
static void imageFadeIn (Scene_t * scene, float progress) ;


static SceneEventManager_t * BOOT_init_events () {
    SceneEventManager_t * manager = create_event_manager(20) ;

    manager->events[manager->eventCount].trigger = imageFadeIn_trigger ;
    manager->events[manager->eventCount++].action = imageFadeIn ;

    return manager ;
}
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

    SceneEventManager_t * eventManager = BOOT_init_events() ;
    if (!existe(eventManager)) {
        destroy_dictionary(&dict);
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

    // Création fonts (TTF_Font) 
    TTF_Font * font8 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 8) ;
    TTF_Font * font16 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 16) ;
    TTF_Font * font24 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 24) ;
    if (!existe(font8) || !existe(font16) || !existe(font16)) {
        fprintf(stderr, "Erreur open font : intro-game/assets/PressStart2P-Regular.ttf\n");
        destroy_dictionary(&self->data);
        return ;
    }

    List_t * listFont = create_list(TTF_CloseFont_cb) ;
    listFont->set(listFont, font8, listFont->size) ;
    listFont->set(listFont, font16, listFont->size) ;
    listFont->set(listFont, font24, listFont->size) ;

    dict->set(dict, "listFont", listFont, destroy_list_cb);

    Dictionary_t * text_dict = create_dictionnary() ;
    if (!existe(text_dict)) {
        fprintf(stderr, "Erreur création dict_textures dans DESKTOP\n");
        destroy_dictionary(&self->data);
        return ;
    }

    if (load_texts_from_file("intro-game/data/donneesTextsBootScene.csv", listFont, text_dict)) {
        fprintf(stderr, "Erreur création des textes dans BOOT\n");
        destroy_dictionary(&text_dict);
        destroy_dictionary(&self->data);
        return ;
    }

    // ajoute text_dict dans data 
    dict->set(dict, "text_dict", text_dict, destroy_dictionary_cb);

    BOOT_Event_t * nameEvent = malloc(sizeof(BOOT_Event_t)) ;
    *nameEvent = LOADING ;
    
    dict->set(dict, "nameEvent", nameEvent, free_cb);

    // recupere la date de lancement en millisecondes
    info->startTime = SDL_GetTicks() ;

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
    Dictionary_t * text_dict = self->data->get(self->data, "text_dict") ;

    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT :
                info->end = TRUE ;
                break;
            case SDL_KEYDOWN :  
                switch (event->key.keysym.sym) {
                    case SDLK_SPACE : 
                        request_scene_change(manager, "DESKTOP");
                        break;
                    default : 
                        break;
                }
            default : 
                break;
        }
    }

    SceneEventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
    process_events(eventManager, self);
}


/**
 * 
 */
void BOOT_update (Scene_t * self, SceneManager_t * manager) {
    
    Dictionary_t * text_dict = self->data->get(self->data, "text_dict") ;
    text_dict_update(text_dict, "intro-game/data/donneesTextsBootScene.csv");

    List_t * listTexture = self->data->get(self->data, "listTexture") ;
    texture_list_update_from_file(listTexture, "intro-game/data/donneesTexturesBootScene.csv");
}


/**
 * 
 */
void BOOT_render (Scene_t * self) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    Dictionary_t * text_dict = self->data->get(self->data, "text_dict") ;
    for (int i = 0; i < text_dict->nbEntry; i++) {
        Entry_t * entry = text_dict->item(text_dict, i) ;
        Text_t * text = entry->value ;

        draw_text(text);
    }

    List_t * listTexture = self->data->get(self->data, "listTexture") ;
    for (int i = 0; i < listTexture->size; i++) {
        Texture_t * texture = listTexture->item(listTexture, i) ;
        
        draw_texture(texture);
    }

    SDL_RenderPresent(renderer); 
}


static float imageFadeIn_trigger (Scene_t * scene, SceneEvent_t * event) {

    InfoScene_t * info = GET_INFO(scene->data) ;

    uint32_t delay = 5000 ;
    uint32_t delayEnd = 8000 ;
    if (info->currentTime >= (info->startTime + delay) && info->currentTime < (info->startTime + delayEnd)) {

        event->active = TRUE ;

        return ((float)(info->currentTime - (info->startTime + delay)) / (float)(delayEnd - delay)) ;
    }
    else {
        event->active = FALSE ;

        return 0.0 ;
    }
}
static void imageFadeIn (Scene_t * scene, float progress) {

    List_t * listTexture = GET_LIST_TEXTURE(scene->data) ;
    Texture_t * texture = listTexture->item(listTexture, I_LOADING_WHEEL) ;
    Texture_t * textureLogo = listTexture->item(listTexture, I_BLIS_FL5) ;

    texture->hidden = FALSE ;
    textureLogo->hidden = FALSE ;

    int alpha = (int)(progress * 255.0) ; 
    
    if (existe(texture) && existe(texture->texture)) {
        SDL_SetTextureAlphaMod(texture->texture, alpha);
    }
    if (existe(texture) && existe(textureLogo->texture)) {
        SDL_SetTextureAlphaMod(textureLogo->texture, alpha);
    }
}
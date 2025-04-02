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


void LEVEL1_load (Scene_t * self) {
    
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return ;
    }

    self->data = create_dictionnary() ;
    if (!existe(self->data)) {
        fprintf(stderr, "Erreur création dictionnaire dans DESKTOP\n");
        return ;
    }

    Dictionary_t * dict = self->data ;

    // Allocation et initialisation du compteur d'éléments
    InfoScene_t * info = malloc(sizeof(InfoScene_t));
    if (info == NULL) {
        fprintf(stderr, "Erreur: allocation mémoire échouée pour `len`\n");
        destroy_dictionary(&self->data);
        return ;
    }
    info->len = 4;
    info->end = FALSE ;

    // Stockage dans `self->data`
    dict->set(dict, "info", info, free_cb);

    printf("[INFO] : Chargement des données LEVEL1 réussi\n");
    
    return ; 
}


void LEVEL1_unLoad (Scene_t * self) {
    
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return;
    }

    if (self->data == NULL) {
        fprintf(stderr, "Aucune donnée à libérer dans `self->data`\n");
        return;
    } 
    
    destroy_dictionary(&self->data);

    printf("[INFO] : Déchargement des données LEVEL1 réussi\n");
    
    return ; 
}


void LEVEL1_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) {
    
    InfoScene_t * info = self->data->get(self->data, "info") ;
    
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT :
                info->end = TRUE ;
                break;
            case SDL_KEYDOWN :
                switch (event->key.keysym.sym) {
                    case SDLK_q :
                        break;
                    case SDLK_d :
                        break;
                    case SDLK_r : 
                        break;
                    case SDLK_SPACE :
                        break;
                    case SDLK_BACKSPACE : 
                        info->end = TRUE;
                    default :
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                break;
            case SDL_MOUSEBUTTONUP :
                break;
            case SDL_MOUSEMOTION : 
                break;
            default : 
                break;
        }
    }
    
    return ; 
}


void LEVEL1_update (Scene_t * self, SceneManager_t * manager) {
    return ; 
}


void LEVEL1_render (Scene_t * self) {

    SDL_Rect dstrect = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, 50, 50} ;
    SDL_SetRenderDrawColor(renderer, 150, 0, 30, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &dstrect);

}
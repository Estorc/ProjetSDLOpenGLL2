#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/camera.h"
#include "../include/render.h"
#include "../include/scene.h"
#include "../include/desktop.h"
#include "../include/text.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"
#include "../include/list.h"


/**
 * @brief Charge les données de la scène "DESKTOP" et les stocke dans `self->data`.
 * 
 * Cette fonction initialise la structure `Scene_t` en allouant et en stockant les données 
 * nécessaires à la scène "DESKTOP". Ces données incluent :
 * - `self->data[0]` : Un pointeur vers une structure `InfoScene_t` qui contient les info de la scene.
 * - `self->data[1]` : Un pointeur vers la structure `Desktop_t`.
 * - `self->data[1]` : Un pointeur vers un font pour les textes (type `TTF_Font`).
 * 
 * Si `self->data` est `NULL`, un tableau dynamique de `void *` est alloué.
 * 
 * @param self Pointeur vers la structure `Scene_t` représentant la scène actuelle.
 * @return int Retourne `1` en cas de succès, ou `0` en cas d'échec.
 * 
 * @note L'appelant est responsable de libérer `self->data`, `len` et `desktop` après utilisation.
 * @warning En cas d'échec de l'allocation mémoire ou du chargement de `desktop`, la fonction retourne `0`.
 */
void DESKTOP_load(Scene_t *self) {
 
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

    dict->set(dict, "info", info, free_cb);

    // Création du bureau (Desktop_t)
    Desktop_t *desktop = create_desktop("intro-game/assets/backgroundDesktopScene.png", "intro-game/assets/objectsDesktopScene.png", "intro-game/data/donneesObjetsDesktopScene.csv", NB_ELEM_DESKTOP_SCENE);
    if (desktop == NULL) {
        fprintf(stderr, "Erreur: création de `desktop` échouée\n");
        destroy_dictionary(&self->data);
        return ;
    }
    // Initialisation element du bureau (DesktopElement_t)
    desktop->elements[3].hidden = TRUE ;    // Annule affichage icone message d'erreur  

    dict->set(dict, "desktop", desktop, destroy_desktop_cb);

    // Création font (TTF_Font) 
    TTF_Font * font = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 12) ;
    if (font == NULL) {
        fprintf(stderr, "Erreur open font : intro-game/assets/PressStart2P-Regular.ttf\n");
        destroy_dictionary(&self->data);
        return ;
    } 

    dict->set(dict, "font", font, TTF_CloseFont_cb);

    // Création tableau de text (`Text_t`) 
    int nbText = 1 ;
    Dictionary_t * text_dict = create_dictionnary() ;
    if (!existe(text_dict)) {
        fprintf(stderr, "Erreur malloc tableau text scene DESKTOP : %s\n", SDL_GetError());
        destroy_dictionary(&self->data);
        return ;
    }

    // création des text 
    SDL_Color blanc = {255, 255, 255, 255} ;
    SDL_Rect position = {50, WINDOW_HEIGHT / 2, 100, 100} ;
    Text_t * text1 = create_text ("je suis nul", font, blanc, position) ;

    SDL_Color rouge = {150, 0, 0, 255} ;
    text_change_hollow(text1, TRUE, rouge, BOTTOM_LEFT);

    text_dict->set(text_dict, "text1", text1, destroy_text_cb);

    dict->set(dict, "text_dict", text_dict, destroy_dictionary_cb);

    printf("[INFO] : Chargement des données DESKTOP réussi\n");
}


/**
 * @brief Décharge les données de la scène "DESKTOP" et libère la mémoire allouée.
 * 
 * Cette fonction libère les ressources allouées dans `DESKTOP_load()`, notamment :
 * - `self->data[0]` : La structure `InfoScene_t * info`.
 * - `self->data[1]` : La structure `Desktop_t`, y compris `desktop->elements` et `desktop->background`.
 * - `self->data[1]` : Le pointeur vers `TTF_Font * font` 
 * - `self->data` : Le tableau de `void *`.
 * 
 * @param self Pointeur vers la structure `Scene_t` contenant les données de la scène.
 * @return int Retourne `1` en cas de succès, ou `0` si `self` est `NULL`.
 */
void DESKTOP_unLoad(Scene_t *self) {
    if (self == NULL) {
        fprintf(stderr, "Erreur: `self` est NULL\n");
        return;
    }
    
    if (self->data == NULL) {
        fprintf(stderr, "Aucune donnée à libérer dans `self->data`\n");
        return;
    }

    destroy_dictionary(&self->data);

    printf("[INFO] : Déchargement des données DESKTOP réussi\n");
}


void DESKTOP_handleEvents (Scene_t * self, SDL_Event * event, SceneManager_t * manager) {

    InfoScene_t * info = self->data->get(self->data, "info") ;
    Desktop_t * desktop = self->data->get(self->data, "desktop") ;
    int iconClicked ;
    
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
                iconClicked = element_update(desktop, event);
                if (iconClicked == ICON_GAME) {
                    request_scene_change(manager, "LEVEL1");
                }
                break;
            case SDL_MOUSEBUTTONUP :
                iconClicked = element_update(desktop, event);
                if (iconClicked == ICON_GAME) {
                    request_scene_change(manager, "LEVEL1");
                }
                break;
            case SDL_MOUSEMOTION : 
                move_element(desktop, event);
                break;
            default : 
                break;
        }
    }
}


void DESKTOP_update (Scene_t * self, SceneManager_t * manager) {

    Dictionary_t * text_dict = self->data->get(self->data, "text_dict") ;
    Text_t * text1 = text_dict->get(text_dict, "text1") ;
    text_update(text1);

    return; 
}


void DESKTOP_render (Scene_t * self) {

    Desktop_t * desktop = self->data->get(self->data, "desktop") ;
    draw_desktop(desktop);

    Dictionary_t * text_dict = self->data->get(self->data, "text_dict") ;
    Text_t * text1 = text_dict->get(text_dict, "text1") ;
    draw_text(text1);

}
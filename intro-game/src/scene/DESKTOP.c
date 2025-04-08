#include "../include/lib.h"
#include "../include/main.h"
#include "../include/window.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/camera.h"
#include "../include/render.h"
#include "../include/scene.h"
#include "../include/gui.h"
#include "../include/text.h"
#include "../include/dictionary.h"
#include "../include/texture_loader.h"
#include "../include/list.h"


uint8_t icon_locked = TRUE ;


static void init_desktop_main_window (Desktop_t * desktop, WinTheme_t theme) ;


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
    info->end = FALSE ;

    dict->set(dict, "info", info, free_cb);


    // Création font (TTF_Font) 
    TTF_Font * font1 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 12) ;
    TTF_Font * font2 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 18) ;
    TTF_Font * font3 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 20) ;
    
    List_t * listFont = create_list(TTF_CloseFont_cb) ;
    listFont->stack(listFont, font1);
    listFont->stack(listFont, font2);
    listFont->stack(listFont, font3);
    
    dict->set(dict, "listFont", listFont, destroy_list_cb);


    // Création du bureau (Desktop_t)
    Desktop_t * desktop = create_desktop() ;
    if (desktop == NULL) {
        fprintf(stderr, "Erreur: création de `desktop` échouée\n");
        destroy_dictionary(&self->data); 
        return ;
    }

    SDL_Color textColor = {255, 255, 255, SDL_ALPHA_OPAQUE} ;
    SDL_Color bgColor = {155, 0, 0, SDL_ALPHA_OPAQUE} ;
    init_desktop_main_window(desktop, (WinTheme_t){font1, textColor, bgColor});
    

    
    
    dict->set(dict, "desktop", desktop, destroy_desktop_cb);
    

    info->startTime = SDL_GetTicks() ;
    info->currentTime = info->startTime ;

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
    
    int actionID ;
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
                        printf("taille list win = %d\n", desktop->listWindow->size) ;
                        break;
                    case SDLK_BACKSPACE : 
                        info->end = TRUE;
                    default :
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :;
                actionID = desktop_element_update(desktop, event) ;
                if (actionID != - 1) desktop_handle_button_events(self, actionID) ;
                break;
            case SDL_MOUSEBUTTONUP :;
                desktop_element_update(desktop, event) ;
                break;
            case SDL_MOUSEMOTION :;
                desktop_move_element(desktop, event) ;
                break;
            default : 
                break;
        }
    }

}


void DESKTOP_update (Scene_t * self, SceneManager_t * manager) {
    return; 
}


void DESKTOP_render (Scene_t * self) {

    Desktop_t * desktop = self->data->get(self->data, "desktop") ;
    draw_desktop(desktop);

}




static
void init_desktop_main_window (Desktop_t * desktop, WinTheme_t theme ) {

    Window_t * window = &desktop->mainWindow ;

    window->background = load_png("intro-game/assets/background3.jpg") ;
    if (!existe(window->background)) {
        printf("Erreur creation background dans load_window_from_file\n"); 
        return ;
    }

    window->spriteSheet = load_png("intro-game/assets/objectsDesktopScene.png") ;
    if (!existe(window->spriteSheet)) {
        printf("Erreur creation spriteSheet dans load_window_from_file\n"); 
        return ;
    }

    window->position.x = 0 ; 
    window->position.y = 0 ; 
    window->position.w = WINDOW_WIDTH ; 
    window->position.h = WINDOW_HEIGHT ;
    
    window->widgetCount = 3 ;
    window->tabWidget = malloc(sizeof(Widget_t) * window->widgetCount) ;
    if (!existe(window->tabWidget)) {
        fprintf(stderr, "Erreur malloc tabWidget : %s\n", SDL_GetError());
        return ;
    }

    init_widgets_from_file(window->tabWidget, "intro-game/data/desktopWidgets.csv", theme);

    window->theme = theme ;

    window->isActive = TRUE ;
    window->isDragged = FALSE ;

    window_change_theme(window, (WinTheme_t){theme.font, theme.textColor, (SDL_Color){0, 0, 0, 0}});

    load_windows_from_file(desktop->listWindow, "intro-game/data/desktopWindows.csv", (WinTheme_t){theme.font, theme.textColor, (SDL_Color){0, 0, 0, 0}});
}
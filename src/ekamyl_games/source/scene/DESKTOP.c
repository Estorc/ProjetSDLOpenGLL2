#include "../../include/lib.h"
#include "../../include/main.h"
#include "../../include/window.h"
#include "../../include/player.h"
#include "../../include/map.h"
#include "../../include/camera.h"
#include "../../include/render.h"
#include "../../include/scene.h"
#include "../../include/gui.h"
#include "../../include/text.h"
#include "../../include/dictionary.h"
#include "../../include/texture_loader.h"
#include "../../include/list.h"
#include "../../include/event.h"


uint8_t icon_locked = TRUE ;

#define STATE_PLAY 0
#define STATE_END 1

#define INDEX_MSG_1 0
#define INDEX_MSG_2 1
#define INDEX_MSG_3 2
#define INDEX_MSG_4 3
#define INDEX_MSG_5 4


// scripts de cette scene 
static float terminate_game_trigger (Scene_t * scene, Event_t * event) ;
static void terminate_game (Scene_t * scene, float progress) ;


// fonction de changement d'etat 
static void change_state (Scene_t * self, InfoScene_t * info) ;


// fonction propre a cette scene 
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

    
    List_t * listFont = create_list(TTF_CloseFont_cb) ;
    if (listFont == NULL) {
        fprintf(stderr, "Erreur creation listFont\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "listFont", listFont, destroy_list_cb);

    // Création font (TTF_Font) 
    TTF_Font * font1 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 12) ;
    TTF_Font * font2 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 18) ;
    TTF_Font * font3 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 20) ;
    TTF_Font * font4 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 24) ;
    TTF_Font * font5 = TTF_OpenFont("intro-game/assets/PressStart2P-Regular.ttf", 28) ;
    listFont->stack(listFont, font1);
    listFont->stack(listFont, font2);
    listFont->stack(listFont, font3);
    listFont->stack(listFont, font4);
    listFont->stack(listFont, font5);
    

    List_t * listText = create_list(destroy_text_cb) ;
    if (listText == NULL) {
        fprintf(stderr, "Erreur creation listText\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "listText", listText, destroy_list_cb) ;

    load_texts_from_file("intro-game/data/textsDesktop.csv", listFont, listText);


    // Création du bureau (Desktop_t)
    Desktop_t * desktop = create_desktop() ;
    if (desktop == NULL) {
        fprintf(stderr, "Erreur: création de `desktop` échouée\n");
        destroy_dictionary(&self->data); 
        return ;
    }
    dict->set(dict, "desktop", desktop, destroy_desktop_cb);

    SDL_Color textColor = {255, 255, 255, SDL_ALPHA_OPAQUE} ;
    SDL_Color bgColor = {155, 0, 0, SDL_ALPHA_OPAQUE} ;
    init_desktop_main_window(desktop, (WinTheme_t){font1, textColor, bgColor});


    EventManager_t * eventManager = create_event_manager() ;
    if (!existe(eventManager)) {
        fprintf(stderr, "Erreur creation eventManager\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "eventManager", eventManager, destroy_event_manager_cb);
    

    if (icon_locked) 
        info->nextState = STATE_PLAY ;
    else 
        info->nextState = STATE_END ;
    
    change_state(self, info);


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

    if (info->state == STATE_END) {

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
                    break;
                case SDL_MOUSEBUTTONUP :;
                    break;
                case SDL_MOUSEMOTION :;
                    break;
                default : 
                    break;
            }
        }

        EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
        process_events(eventManager, self);

        return ;
    }
    
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


    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
    process_events(eventManager, self);

    return ;
}


void DESKTOP_update (Scene_t * self, SceneManager_t * manager) {

    InfoScene_t * info = self->data->get(self->data, "info") ;

    if (info->state == STATE_END) {

        List_t * listText = GET_LIST_TEXT(self->data) ;
        List_t * listFont = GET_LIST_FONT(self->data) ;

        Text_t * text1 = listText->item(listText, INDEX_MSG_1) ;
        Text_t * text2 = listText->item(listText, INDEX_MSG_2) ;
        Text_t * text3 = listText->item(listText, INDEX_MSG_3) ;
        Text_t * text4 = listText->item(listText, INDEX_MSG_4) ;
        Text_t * text5 = listText->item(listText, INDEX_MSG_5) ;
        if (text1->animation.playing == FALSE) {
            text2->hidden = FALSE ;
        }
        if (text2->animation.playing == FALSE && info->currentTime - info->startTime >= 5000) {
            text1->hidden = TRUE ;
            text2->hidden = TRUE ;
            text3->hidden = FALSE ;
        }
        if (text3->animation.playing == FALSE) {
            text4->hidden = FALSE ;
        }
        if (text4->animation.playing == FALSE) {
            text5->hidden = FALSE ;
        }

        text_list_update_from_file(listText, listFont, "intro-game/data/textsDesktop.csv");
    }

    if (info->nextState != info->state) {
        printf("changement de state\n");
        change_state(self, info);
    }

    return; 
}


void DESKTOP_render (Scene_t * self) {

    InfoScene_t * info = self->data->get(self->data, "info") ;

    if (info->state == STATE_PLAY) {

        Desktop_t * desktop = self->data->get(self->data, "desktop") ;
        draw_desktop(desktop);
    
        List_t * listFont = GET_LIST_FONT(self->data) ;
        draw_time((SDL_Rect){822, 652, 0, 0}, listFont->item(listFont, 0), (SDL_Color){202, 202, 202, SDL_ALPHA_OPAQUE});
    }
    if (info->state == STATE_END) {

        Desktop_t * desktop = self->data->get(self->data, "desktop") ;
        draw_desktop(desktop);

        // assombri l'ecran 
        SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT} ;
        draw_rect_filled(rect, (SDL_Color){0, 0, 0, 130});

        List_t * listText = GET_LIST_TEXT(self->data) ;
        for (int i = 0; i < listText->size; i++) {
            draw_text(listText->item(listText, i));
        }
    }
}




static 
void change_state (Scene_t * self, InfoScene_t * info) {

    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;

    switch (info->nextState) {

    case STATE_PLAY :;

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;

    case STATE_END :;

        List_t * listText = GET_LIST_TEXT(self->data) ;

        Text_t * text1 = listText->item(listText, INDEX_MSG_1) ;
        text1->hidden = FALSE ;

        add_event(eventManager, 15000, 0, terminate_game_trigger, terminate_game);

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;
    
    default:
        break;
    }
}


static
float terminate_game_trigger (Scene_t * scene, Event_t * event) {
    
    InfoScene_t * info = GET_INFO(scene->data) ;
    
    if (info->currentTime >= event->execTime) {

        return (float)(info->currentTime - event->execTime) / (float)(event->duration) ;
    }

    return 0.0f ;
}
static
void terminate_game (Scene_t * scene, float progress) {

    InfoScene_t * info = GET_INFO(scene->data) ; 

    info->end = TRUE ;
}


static
void init_desktop_main_window (Desktop_t * desktop, WinTheme_t theme ) {

    Window_t * window = &desktop->mainWindow ;

    window->background = load_png("intro-game/assets/desktop_glitch2.png") ;
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
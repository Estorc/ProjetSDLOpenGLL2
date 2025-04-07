#include "../include/lib.h"
#include "../include/main.h"
#include "../include/texture_loader.h"
#include "../include/dictionary.h"
#include "../include/list.h"
#include "../include/gui.h"
#include "../include/text.h"
#include "../include/scene.h"

// id de la liste de fenetre pour differencier chaque fenetre
static uint8_t windowId = 0 ;

/**
 * Creer une structure de type `Desktop_t`. L'intialisation de ses attributs devra se faire 
 * end ehors de cette focntion.
 */
Desktop_t * create_desktop () {
    
    // Allocation mémoire de la structure Desktop_t
    Desktop_t * desktop = (Desktop_t *) malloc(sizeof(Desktop_t));
    if (desktop == NULL) { 
        fprintf(stderr, "Erreur malloc desktop");
        return NULL;
    }

    Window_t * mainWindow = &desktop->mainWindow ;
    mainWindow->background = NULL ;
    mainWindow->spriteSheet = NULL ;
    mainWindow->tabWidget = NULL ;
    mainWindow->widgetCount = 0 ;
    mainWindow->isActive = TRUE ;

    desktop->listWindow = create_list(destroy_window_cb) ;
    

    return desktop;
}


/**
 * Detruis la structure `Desktop_t` et tout ses attributs 
 */
void destroy_desktop(Desktop_t **desktop) {

    if (desktop != NULL && existe(desktop)) {

        // Destruction liste de fenetre 
        destroy_list( &(*desktop)->listWindow );


        // Destruction bureau 
        Window_t * window = &(*desktop)->mainWindow ;
        if ( existe( window->background ) ) {

            SDL_DestroyTexture(window->background);
            window->background = NULL ;
        }

        if ( existe( window->spriteSheet ) ) {

            SDL_DestroyTexture(window->spriteSheet);
            window->spriteSheet = NULL ;
        }

        if ( existe( window->tabWidget ) ) {

            // Supprime chaque texture et string des widget de type text 
            for (int i = 0; i < window->widgetCount; i++) {
                if (window->tabWidget[i].type == WIDGET_TEXT) {

                    SDL_DestroyTexture(window->tabWidget[i].text.texture);
                    free(window->tabWidget[i].text.string);
                }
            }

            free(window->tabWidget);
            window->tabWidget = NULL ;
        }

        free(*desktop);
        *desktop = NULL ;
    }
}
void destroy_desktop_cb (void * desktop) {
    destroy_desktop(desktop);
}


Window_t * create_window () {

    Window_t * window = malloc(sizeof(Window_t)) ;
    if (!existe(window)) {
        fprintf(stderr, "Erreur malloc window\n");
        return NULL ;
    }

    window->background = NULL ;
    window->spriteSheet = NULL ;
    window->tabWidget = NULL ;
    window->widgetCount = 0 ;
    window->isActive = FALSE ;
    window->isDragged = FALSE ;
    
    return window ;
}


void init_desktop_main_window (Window_t * window, WinTheme_t theme ) {

    window->background = load_png("intro-game/assets/backgroundDesktopScene.png") ;
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
    
    window->widgetCount = 4 ;
    window->tabWidget = malloc(sizeof(Widget_t) * window->widgetCount) ;
    if (!existe(window->tabWidget)) {
        fprintf(stderr, "Erreur malloc tabWidget : %s\n", SDL_GetError());
        return ;
    }

    init_widgets_from_file(window->tabWidget, "intro-game/data/desktopWidgets.csv", theme);

    window->theme = theme ;

    window->isActive = TRUE ;
    window->isDragged = FALSE ;
}


/**
 * Creer une structure `Window_t` puis initialise ses attributs a partir d'un fichier.
 */
void load_windows_from_file (List_t * list, char * dataPath, WinTheme_t theme) {

    FILE * file = fopen(dataPath, "r") ;
    if (!existe(file)) {
        fprintf(stderr, "Erreur ouverture fichier \"%s\" : %s\n", dataPath, SDL_GetError());
        return ;
    }

    char backgroundPath[256], spriteSheetPath[256], widgetsPath[256] ;
    int x, y, w, h ;
    int widgetCount ;
    
    char buffer[256] ;
    fgets(buffer, sizeof(buffer), file);

    while (fscanf(file, "%[^;];%[^;];%[^;];%d;%d;%d;%d;%d", backgroundPath, spriteSheetPath, widgetsPath, &widgetCount, &x, &y, &w, &h) == 8) {

        Window_t * window = create_window() ;
        if (!existe(window)) {
            printf("Erreur creation window dans load_window_from_file\n"); 
            return ;
        }

        window->background = load_png(backgroundPath) ;
        if (!existe(window->background)) {
            printf("Erreur creation background dans load_window_from_file\n"); 
            return ;
        }

        window->spriteSheet = load_png(spriteSheetPath) ;
        if (!existe(window->spriteSheet)) {
            printf("Erreur creation spriteSheet dans load_window_from_file\n"); 
            return ;
        }

        window->position.x = x ; 
        window->position.y = y ; 
        window->position.w = w ; 
        window->position.h = h ;
        
        window->tabWidget = malloc(sizeof(Widget_t) * widgetCount) ;
        if (!existe(window->tabWidget)) {
            fprintf(stderr, "Erreur malloc tabWidget : %s\n", SDL_GetError());
            return ;
        }

        init_widgets_from_file(window->tabWidget, widgetsPath, theme);
        window->widgetCount = widgetCount ;

        window->theme = theme ;

        window->isActive = FALSE ;
        window->isDragged = FALSE ;

        window->id = windowId++ ;

        list->stack(list, window);
    }

    fclose(file);


    return ;
}


void destroy_window (Window_t ** window) {

    if (window != NULL && existe(*window)) {

        if ( existe( (*window)->background ) ) {

            SDL_DestroyTexture((*window)->background);
            (*window)->background = NULL ;
        }

        if ( existe( (*window)->spriteSheet ) ) {

            SDL_DestroyTexture((*window)->spriteSheet);
            (*window)->spriteSheet = NULL ;
        }

        if ( existe( (*window)->tabWidget ) ) {

            // Supprime chaque texture et string des widget de type text 
            for (int i = 0; i < (*window)->widgetCount; i++) {
                if ((*window)->tabWidget[i].type == WIDGET_TEXT) {

                    SDL_DestroyTexture((*window)->tabWidget[i].text.texture);
                    free((*window)->tabWidget[i].text.string);
                }
            }

            free((*window)->tabWidget);
            (*window)->tabWidget = NULL ;
        }

        free(*window);
        *window = NULL ;
    }
}
void destroy_window_cb (void * window) {
    destroy_window(window);
}


void window_change_theme (Window_t * window, WinTheme_t newTheme) {

    window->theme.font = newTheme.font ;
    window->theme.textColor = newTheme.textColor ;
    window->theme.bgColor = newTheme.bgColor ;

    Widget_t * tab = window->tabWidget ;
    for (int i = 0; i < window->widgetCount; i++) {
        
        if (tab[i].type == WIDGET_TEXT && existe(tab[i].text.string)) {
            SDL_DestroyTexture(tab[i].text.texture);
            tab[i].text.texture = create_TTF_Texture(newTheme.font, tab[i].text.string, newTheme.textColor) ;
        }
    }
}


void window_add_widget (Window_t * window, Widget_t newWidget) {

    if (!existe(window)) {
        printf("Impossible d'ajouter un widget, car window inexistant\n");
        return ;
    }

    
    Widget_t * newTab = realloc(window->tabWidget, sizeof(Widget_t) * (window->widgetCount + 1)) ;
    if (existe(newTab)) {
        window->tabWidget = newTab ;
        window->tabWidget[window->widgetCount++] = newWidget ;
    }
    else {
        fprintf(stderr, "Erreur realloc de tabWidget, le newWidget n'a pas pu etre ajouté\n");
        return ;
    }

}


void init_widgets_from_file (Widget_t * tab, char * dataPath, WinTheme_t theme) {

    FILE * file = fopen(dataPath, "r") ;
    if (!existe(file)) {
        fprintf(stderr, "Erreur ouverture fichier \"%s\" : %s\n", dataPath, SDL_GetError());
        return ;
    }

    int x1, y1, w1, h1 ;
    int x2, y2, w2, h2 ;
    int type ;
    
    char buffer[256] ;
    fgets(buffer, sizeof(buffer), file);

    int i = 0 ;
    while (fscanf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d", 
                &type, &x1, &y1, &w1, &h1, &x2, &y2, &w2, &h2) == 9) 
        {
        
        tab[i].relPosition.x = x1 ;
        tab[i].relPosition.y = y1 ;
        tab[i].relPosition.w = w1 ;
        tab[i].relPosition.h = h1 ;

        tab[i].type = type ;

        switch (type) {
        case WIDGET_BUTTON :
            tab[i].button.srcrect.x = x2 ;
            tab[i].button.srcrect.y = y2 ;
            tab[i].button.srcrect.w = w2 ;
            tab[i].button.srcrect.h = h2 ;

            if (fscanf(file, ";%d", (int *)&tab[i].button.actionID) != 1) {
                printf("Erreur fscanf de cbFunc dans init_widgets_from_file\n");
                return ;
            }
            break;
        
        case WIDGET_ICON :
            tab[i].icon.srcrect.x = x2 ;
            tab[i].icon.srcrect.y = y2 ;
            tab[i].icon.srcrect.w = w2 ;
            tab[i].icon.srcrect.h = h2 ;

            tab[i].icon.isClicked = FALSE ;
            tab[i].icon.isDragged = FALSE ;

            if (fscanf(file, ";%d", (int *)&tab[i].icon.actionID) != 1) {
                printf("Erreur fscanf de cbFunc dans init_widgets_from_file\n");
                return ;
            }

            break;

        case WIDGET_TEXT :;
            char text[256] ;
            if (fscanf(file, ";%[^\n]", text) != 1) 
            {
                fprintf(stderr, "Erreur fscanf de WIDGET_TEXT dans init_widgets_from_file\n");
                return ;
            }
            tab[i].text.string = strdup(text) ;

            tab[i].text.texture = create_TTF_Texture(theme.font, text, theme.textColor) ;
            if (!existe(tab[i].text.texture)) {
                printf("Erreur creation ttf texture dans init_widgets_from_file\n");
                return ;
            }

            break;

        default:
            break;
        }

        i++;
    }

    fclose(file);

    return ;
}



/**
 * @brief Détecte l'interaction de l'utilisateur avec un élément du bureau.
 * 
 * Cette fonction vérifie si la souris est positionnée sur un élément du bureau
 * et met à jour son état en fonction de l'événement SDL reçu.
 */
int desktop_element_update(Desktop_t *desktop, SDL_Event *event) {

    if (desktop == NULL || event == NULL) {
        fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
        return -1;
    } 

    // Récupération de la position actuelle de la souris
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    
    // Cherche la fenetre active la plus haute 
    Window_t * window = &desktop->mainWindow ;
    List_t * listWindow = desktop->listWindow ;
    uint8_t run = TRUE ;
    for (int i = listWindow->size - 1; i >= 0 && run; i--) {
        
        Window_t * currWindow = listWindow->item(listWindow, i) ;
        if (currWindow->isActive && SDL_PointInRect(&mouse, &currWindow->position)) {
            window = currWindow ;
            run = FALSE ;

            if (i != listWindow->size - 1) {
                listWindow->swap(listWindow, i, listWindow->size - 1);
            }
        }
    }


    // Verifie si un des widget a été cliquée et update son état si besoin 
    Widget_t * tab = window->tabWidget ;
    for (int i = 0; i < window->widgetCount; i++) {

        Widget_t * currWidget = &tab[i] ;
        SDL_Rect absPosition = {
            currWidget->relPosition.x + window->position.x, 
            currWidget->relPosition.y + window->position.y, 
            currWidget->relPosition.w, 
            currWidget->relPosition.h
        };
        switch (currWidget->type) {

        case WIDGET_ICON :;
            if (SDL_PointInRect(&mouse, &absPosition)) {
                if (event->type == SDL_MOUSEBUTTONDOWN) {
                    currWidget->icon.isDragged = TRUE;  // L'élément est en cours de déplacement
                    currWidget->icon.isClicked = TRUE;  // L'élément est cliqué
                }
                else if (event->type == SDL_MOUSEBUTTONUP) {
                    currWidget->icon.isDragged = FALSE; // L'élément est relâché
                    currWidget->icon.isClicked = TRUE;  // L'élément a bien été cliqué
                }
        
                // Vérifie si l'utilisateur a effectué un double-clic sur l'élément
                if (event->button.clicks == 2) {
                    return currWidget->icon.actionID ;
                }
            }
            else {
                // Réinitialisation des états si l'élément n'est pas sous la souris
                currWidget->icon.isDragged = FALSE;
                currWidget->icon.isClicked = FALSE;
            }   
            break;
        
        case WIDGET_BUTTON :;
            if (SDL_PointInRect(&mouse, &absPosition)) {
                if (i == CLOSE_BUTTON) {
                    window->isActive = FALSE ;
                    window->isDragged = FALSE ;
                }
                return currWidget->button.actionID ;
            } 
            break;

        default :
            break;
        }
    }

    // Rect de la bar superieur de la fenetre pour la deplacer 
    SDL_Rect rect = {
        window->position.x,
        window->position.y,
        window->position.w,
        50
    };
    if (SDL_PointInRect(&mouse, &rect) && event->type == SDL_MOUSEBUTTONDOWN) {
        window->isDragged = TRUE ;
    }
    else {
        window->isDragged = FALSE ;
    }


    return -1; // Aucun élément n'a été activé
}


/**
 * @brief Permet de déplacer un élément du bureau lorsque l'utilisateur clique et glisse avec la souris.
 * 
 * Cette fonction détecte si la souris est positionnée sur un élément du bureau et affiche un effet visuel 
 * lorsqu'elle le survole. Si l'élément est en mode "dragged", il suit les mouvements relatifs de la souris.
 * 
 * @param desktop Pointeur vers la structure `Desktop_t` contenant les éléments affichés à l'écran.
 * @param event Pointeur vers la structure `SDL_Event` contenant les informations sur l'événement utilisateur.
 */
void desktop_move_element(Desktop_t *desktop, SDL_Event *event) {
    
    if (desktop == NULL || event == NULL) {
        fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
        return;
    }

    // Récupération de la position actuelle de la souris
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    
    // Déplacement des icones du bureau 
    Window_t * window = &desktop->mainWindow ;
    Widget_t * tab = window->tabWidget ;
    for (int i = 0; i < window->widgetCount; i++) {

        Widget_t * currWidget = &tab[i] ;
        if (currWidget->type == WIDGET_ICON && currWidget->icon.isDragged) {
            currWidget->relPosition.x += event->motion.xrel ;
            currWidget->relPosition.y += event->motion.yrel ;
        }
    }


    // Déplacement des fenetres 
    List_t * listWindow = desktop->listWindow ;
    for (int i = 0; i < listWindow->size; i++) {

        Window_t * currWindow = listWindow->item(listWindow, i) ;
        if (currWindow->isActive && currWindow->isDragged) {
            currWindow->position.x += event->motion.xrel ;
            currWindow->position.y += event->motion.yrel ;
        }
    }
}


Window_t * desktop_get_window_from_id (Desktop_t * desktop, uint8_t id) {
    
    if (!existe(desktop)) {
        printf("impossible de recupere le window car desktop est NULL\n");
        return NULL ;
    }

    List_t * list = desktop->listWindow ;
    for (int i = 0; i < list->size; i++) {

        Window_t * window = list->item(list, i) ;
        if (window->id == id) 
            return window ;
    }

    return NULL ;
}


/**
 * Effectue l'action suite au clic d'un bouton
 */
void desktop_handle_button_events (Scene_t * scene, uint8_t actionID) {

    Desktop_t * desktop = GET_DESKTOP(scene->data) ;

    switch(actionID) {
    
    case 0 :
        printf("action 0 effectuée\n");
        Window_t * window = desktop_get_window_from_id(desktop, 0) ;
        if (existe(window)) {
            window->isActive = TRUE ;
        }
        break;

    case 1 :
        printf("action 1 effectuée\n");
        request_scene_change(sceneManager, "LEVEL1");
        break;

    default :
        printf("action par defaut\n");
        break;
    }
}
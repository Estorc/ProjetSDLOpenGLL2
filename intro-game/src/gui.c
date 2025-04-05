#include "../include/lib.h"
#include "../include/main.h"
#include "../include/texture_loader.h"
#include "../include/list.h"
#include "../include/gui.h"
#include "../include/text.h"


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

        destroy_list( &(*desktop)->listWindow );

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
    
    window->widgetCount = 3 ;
    window->tabWidget = malloc(sizeof(Widget_t) * window->widgetCount) ;
    if (!existe(window->tabWidget)) {
        fprintf(stderr, "Erreur malloc tabWidget : %s\n", SDL_GetError());
        return ;
    }

    init_widgets_from_file(window->tabWidget, "intro-game/data/desktopWidgets.csv", theme);

    window->theme = theme ;

    window->isActive = TRUE ;
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

    while (fscanf(file, "%s;%s;%s;%d;%d;%d;%d;%d", backgroundPath, spriteSheetPath, widgetsPath, &widgetCount, &x, &y, &w, &h) == 8) {

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
            tab[i].text.texture = create_TTF_Texture(newTheme.font, tab[i].text.string, newTheme.textColor) ;
        }
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
    int cbFunc ;
    
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

            tab[i].button.isClicked = FALSE ;

            if (fscanf(file, ";%d", &cbFunc) != 1) {
                printf("Erreur fscanf de cbFunc dans init_widgets_from_file\n");
                return ;
            }

            //init_button_onClick(&tab[i].button.onClick, cbFunc);
            break;
        
        case WIDGET_ICON :
            tab[i].icon.srcrect.x = x2 ;
            tab[i].icon.srcrect.y = y2 ;
            tab[i].icon.srcrect.w = w2 ;
            tab[i].icon.srcrect.h = h2 ;

            tab[i].icon.isClicked = FALSE ;
            tab[i].icon.isDragged = FALSE ;

            if (fscanf(file, ";%d", &cbFunc) != 1) {
                printf("Erreur fscanf de cbFunc dans init_widgets_from_file\n");
                return ;
            }

            //init_icon_onClick(&tab[i].icon.onClick, cbFunc);
            break;

        case WIDGET_TEXT :
            tab[i].text.rect.x = x2 ;
            tab[i].text.rect.y = y2 ;
            tab[i].text.rect.w = w2 ;
            tab[i].text.rect.h = h2 ;

            char text[256] ;
            if (fscanf(file, ";%s", text) != 1) 
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



// /**
//  * @brief Détecte l'interaction de l'utilisateur avec un élément du bureau.
//  * 
//  * Cette fonction vérifie si la souris est positionnée sur un élément du bureau
//  * et met à jour son état en fonction de l'événement SDL reçu.
//  * 
//  * @param desktop Pointeur vers la structure `Desktop_t` contenant les éléments affichés à l'écran.
//  * @param event Pointeur vers la structure `SDL_Event` représentant l'événement utilisateur (ex: clic souris).
//  * @return int L'index de l'élément activé (double-cliqué), ou `-1` si aucun élément n'a été exécuté.
//  * 
//  * @note La fonction met à jour les attributs `dragged` et `clicked` des éléments en fonction de l'interaction.
//  */
// int element_update(Desktop_t *desktop, SDL_Event *event) {
//     if (desktop == NULL || event == NULL) {
//         fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
//         return -1;
//     } 

//     // Récupération de la position actuelle de la souris
//     SDL_Point mouse;
//     SDL_GetMouseState(&mouse.x, &mouse.y);

//     // Parcourt tous les éléments du bureau
//     for (int i = 0; i < desktop->nbElem; i++) {
//         // Vérifie si l'élément est visible et si la souris est dessus
//         if (!desktop->elements[i].hidden && SDL_PointInRect(&mouse, &desktop->elements[i].position)) {

//             // Gestion des événements liés à la souris
//             if (event->type == SDL_MOUSEBUTTONDOWN) {
//                 desktop->elements[i].dragged = TRUE;  // L'élément est en cours de déplacement
//                 desktop->elements[i].clicked = TRUE;  // L'élément est cliqué
//             }
//             else if (event->type == SDL_MOUSEBUTTONUP) {
//                 desktop->elements[i].dragged = FALSE; // L'élément est relâché
//                 desktop->elements[i].clicked = TRUE;  // L'élément a bien été cliqué
//             }

//             // Vérifie si l'utilisateur a effectué un double-clic sur l'élément
//             if (event->button.clicks == 2) {
//                 return i; // Retourne l'index de l'élément activé
//             }
//         }
//         else {
//             // Réinitialisation des états si l'élément n'est pas sous la souris
//             desktop->elements[i].dragged = FALSE;
//             desktop->elements[i].clicked = FALSE;
//         }
//     }

//     return -1; // Aucun élément n'a été activé
// }


// /**
//  * @brief Permet de déplacer un élément du bureau lorsque l'utilisateur clique et glisse avec la souris.
//  * 
//  * Cette fonction détecte si la souris est positionnée sur un élément du bureau et affiche un effet visuel 
//  * lorsqu'elle le survole. Si l'élément est en mode "dragged", il suit les mouvements relatifs de la souris.
//  * 
//  * @param desktop Pointeur vers la structure `Desktop_t` contenant les éléments affichés à l'écran.
//  * @param event Pointeur vers la structure `SDL_Event` contenant les informations sur l'événement utilisateur.
//  */
// void move_element(Desktop_t *desktop, SDL_Event *event) {
//     if (desktop == NULL || event == NULL) {
//         fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
//         return;
//     }

//     // Récupération de la position actuelle de la souris
//     SDL_Point mouse;
//     SDL_GetMouseState(&mouse.x, &mouse.y);

//     // Activation du mode de fusion (blending) pour permettre la transparence des couleurs rendues
//     SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//     SDL_SetRenderDrawColor(renderer, 231, 244, 244, 60); // Couleur semi-transparente pour l'effet visuel

//     // Parcours de tous les éléments du bureau
//     for (int i = 0; i < desktop->nbElem; i++) {
//         // Vérifie si l'élément est visible et si la souris est dessus
//         if (!desktop->elements[i].hidden && SDL_PointInRect(&mouse, &desktop->elements[i].position)) {
//             SDL_RenderFillRect(renderer, &desktop->elements[i].position); // Affiche un effet visuel
//         }

//         // Si l'élément est en mode "dragged", il suit les déplacements de la souris
//         if (desktop->elements[i].dragged) {
//             desktop->elements[i].position.x += event->motion.xrel; // Déplacement horizontal
//             desktop->elements[i].position.y += event->motion.yrel; // Déplacement vertical
//         }
//     }
// }
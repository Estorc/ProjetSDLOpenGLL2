#include "../include/lib.h"
#include "../include/main.h"
#include "../include/desktop.h"
#include "../include/texture_loader.h"


/**
 * @brief Crée et initialise une structure `Desktop_t` à partir d'un fichier de configuration.
 * 
 * Cette fonction alloue dynamiquement une structure `Desktop_t`, charge une texture de fond
 * et initialise un tableau d'éléments `DesktopElementt_t` en lisant les données depuis un fichier.
 * 
 * @param backgroundPath Chemin vers l'image de fond à charger en tant que texture SDL.
 * @param dataPath Chemin du fichier contenant les données des éléments du bureau.
 * @param nbElements Nombre d'éléments `DesktopElementt_t` à initialiser.
 * @return Desktop_t* Pointeur vers la structure `Desktop_t` allouée et initialisée, ou `NULL` en cas d'échec.
 * 
 * @note L'utilisateur est responsable de libérer la mémoire avec `free(desktop->elements)`, 
 *       `SDL_DestroyTexture(desktop->background)`, et `free(desktop)`.
 */
Desktop_t * create_desktop(char *backgroundPath, char * elementSpritesheetPath, char *dataPath, int nbElements) {
    
    // Allocation mémoire de la structure Desktop_t
    Desktop_t *desktop = (Desktop_t *) malloc(sizeof(Desktop_t));
    if (desktop == NULL) { 
        fprintf(stderr, "Erreur malloc desktop : %s\n", dataPath);
        return NULL;
    }

    // Chargement de la texture de fond
    desktop->background = load_png(backgroundPath);
    if (desktop->background == NULL) {
        free(desktop); // Libération de la mémoire en cas d'échec
        return NULL;
    }
    
    
    // Chargement de la texture des elements 
    desktop->elementSpritesheet = load_png(elementSpritesheetPath) ;
    if (desktop->elementSpritesheet == NULL) {
        SDL_DestroyTexture(desktop->background);
        free(desktop);
        return NULL;
    }

    
    // Ouverture du fichier contenant les données des éléments
    FILE *dataFile = fopen(dataPath, "r");
    if (dataFile == NULL) {
        fprintf(stderr, "Erreur fopen %s\n", dataPath);
        SDL_DestroyTexture(desktop->background);
        SDL_DestroyTexture(desktop->elementSpritesheet);
        free(desktop);
        return NULL;
    } 


    // Allocation mémoire pour le tableau d'éléments
    desktop->elements = (DesktopElementt_t *) malloc(sizeof(DesktopElementt_t) * nbElements);
    if (desktop->elements == NULL) {
        fprintf(stderr, "Erreur malloc desktop->elements de %s\n", dataPath);
        SDL_DestroyTexture(desktop->background);
        SDL_DestroyTexture(desktop->elementSpritesheet);
        fclose(dataFile);
        free(desktop);
        return NULL;
    }

    // Variables pour stocker les données lues
    int srcX, srcY, srcW, srcH;
    int dstX, dstY, dstW, dstH;
    char buffer[256];

    // Lecture de la première ligne
    fgets(buffer, sizeof(buffer), dataFile);

    // Lecture et initialisation des éléments
    for (int i = 0; i < nbElements; i++) {
        if (fscanf(dataFile, " %d;%d;%d;%d;%d;%d;%d;%d", 
                   &srcX, &srcY, &srcW, &srcH, &dstX, &dstY, &dstW, &dstH) != 8) {
            fprintf(stderr, "Erreur de lecture dans %s à l'élément %d\n", dataPath, i);
            break;
        }

        desktop->elements[i].srcrect.x = srcX ;
        desktop->elements[i].srcrect.y = srcY ;
        desktop->elements[i].srcrect.w = srcW ;
        desktop->elements[i].srcrect.h = srcH ;

        desktop->elements[i].position.x = dstX ;
        desktop->elements[i].position.y = dstY ;
        desktop->elements[i].position.w = dstW ;
        desktop->elements[i].position.h = dstH ;

        // Initialisation des flags
        desktop->elements[i].hidden = FALSE ;
        desktop->elements[i].clicked = FALSE ;
        desktop->elements[i].dragged = FALSE ;
    }

    desktop->nbElem = nbElements ;

    fclose(dataFile);

    return desktop;
}


/**
 * @brief Libère la mémoire allouée pour un bureau (`Desktop_t`) et ses ressources associées.
 * 
 * Cette fonction s'assure que toutes les ressources dynamiquement allouées (éléments du bureau, texture)
 * sont bien libérées avant de désallouer la structure `Desktop_t` elle-même.
 * 
 * @param desktop Double pointeur vers la structure `Desktop_t` à détruire.
 *                Après l'appel, *desktop est mis à `NULL` pour éviter toute référence invalide.
 */
void destroy_desktop(Desktop_t **desktop) {
    // Vérifie si le pointeur est valide avant d'accéder à la mémoire
    if (desktop != NULL && *desktop != NULL) {

        // Libère la mémoire des éléments si elle a été allouée
        if ((*desktop)->elements != NULL) {
            free((*desktop)->elements);  
            (*desktop)->elements = NULL; // Évite les accès mémoire invalides
        }

        // Détruit la texture de fond si elle existe
        if ((*desktop)->background != NULL) {
            SDL_DestroyTexture((*desktop)->background);
        }

        // Détruit la texture des éléments 
        if ((*desktop)->elementSpritesheet != NULL) {
            SDL_DestroyTexture((*desktop)->elementSpritesheet);
        }

        // Libère la structure principale Desktop_t
        free(*desktop);
        *desktop = NULL; // Évite toute référence à un pointeur dangling

        printf("[INFO] : Désalocation structure Desktop réussie\n");
    }
}
void destroy_desktop_cb (void * desktop) {
    Desktop_t ** pdesktop = (Desktop_t **)desktop ;
    destroy_desktop(pdesktop);
}



/**
 * @brief Détecte l'interaction de l'utilisateur avec un élément du bureau.
 * 
 * Cette fonction vérifie si la souris est positionnée sur un élément du bureau
 * et met à jour son état en fonction de l'événement SDL reçu.
 * 
 * @param desktop Pointeur vers la structure `Desktop_t` contenant les éléments affichés à l'écran.
 * @param event Pointeur vers la structure `SDL_Event` représentant l'événement utilisateur (ex: clic souris).
 * @return int L'index de l'élément activé (double-cliqué), ou `-1` si aucun élément n'a été exécuté.
 * 
 * @note La fonction met à jour les attributs `dragged` et `clicked` des éléments en fonction de l'interaction.
 */
int element_update(Desktop_t *desktop, SDL_Event *event) {
    if (desktop == NULL || event == NULL) {
        fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
        return -1;
    }

    // Récupération de la position actuelle de la souris
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    // Parcourt tous les éléments du bureau
    for (int i = 0; i < desktop->nbElem; i++) {
        // Vérifie si l'élément est visible et si la souris est dessus
        if (!desktop->elements[i].hidden && SDL_PointInRect(&mouse, &desktop->elements[i].position)) {

            // Gestion des événements liés à la souris
            if (event->type == SDL_MOUSEBUTTONDOWN) {
                desktop->elements[i].dragged = TRUE;  // L'élément est en cours de déplacement
                desktop->elements[i].clicked = TRUE;  // L'élément est cliqué
            }
            else if (event->type == SDL_MOUSEBUTTONUP) {
                desktop->elements[i].dragged = FALSE; // L'élément est relâché
                desktop->elements[i].clicked = TRUE;  // L'élément a bien été cliqué
            }

            // Vérifie si l'utilisateur a effectué un double-clic sur l'élément
            if (event->button.clicks == 2) {
                return i; // Retourne l'index de l'élément activé
            }
        }
        else {
            // Réinitialisation des états si l'élément n'est pas sous la souris
            desktop->elements[i].dragged = FALSE;
            desktop->elements[i].clicked = FALSE;
        }
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
void move_element(Desktop_t *desktop, SDL_Event *event) {
    if (desktop == NULL || event == NULL) {
        fprintf(stderr, "Erreur: `desktop` ou `event` est NULL\n");
        return;
    }

    // Récupération de la position actuelle de la souris
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    // Activation du mode de fusion (blending) pour permettre la transparence des couleurs rendues
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 231, 244, 244, 60); // Couleur semi-transparente pour l'effet visuel

    // Parcours de tous les éléments du bureau
    for (int i = 0; i < desktop->nbElem; i++) {
        // Vérifie si l'élément est visible et si la souris est dessus
        if (!desktop->elements[i].hidden && SDL_PointInRect(&mouse, &desktop->elements[i].position)) {
            SDL_RenderFillRect(renderer, &desktop->elements[i].position); // Affiche un effet visuel
        }

        // Si l'élément est en mode "dragged", il suit les déplacements de la souris
        if (desktop->elements[i].dragged) {
            desktop->elements[i].position.x += event->motion.xrel; // Déplacement horizontal
            desktop->elements[i].position.y += event->motion.yrel; // Déplacement vertical
        }
    }
}

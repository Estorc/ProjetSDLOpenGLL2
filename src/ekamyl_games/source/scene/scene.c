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



/**
 * Detruis la scene 
 */
void destroy_scene (Scene_t ** scene) {

    if (existe(scene) && existe(*scene)) {

        if (existe((*scene)->name)) {
            
            // libere memoire chaine caractere 
            free((*scene)->name);
            (*scene)->name = NULL;
        }

        // libere memoire structure Scene_t
        free(*scene);
        *scene = NULL;
    }
    else {
        printf("destruction scene impossible \n");
    }
}


/**
 * Creer une nouvelle scene 
 */
Scene_t * create_scene (char * name, void (*load) (Scene_t *), void (*unLoad) (Scene_t *),
                        void (*handleEvents) (Scene_t *, SDL_Event *, SceneManager_t *), void (*update) (Scene_t *, SceneManager_t *), 
                        void (*render) (Scene_t *)) {
    
    // alloue memoire scene
    Scene_t * scene = malloc(sizeof(Scene_t));
    if (scene == NULL) {
        fprintf(stderr, "Erreur malloc scene : %s\n", name);
        return NULL;
    }

    
    // recopie du parametre name dans la scene 
    int len = strlen(name);
    scene->name = malloc(sizeof(char) * len + 1);
    if (scene->name == NULL) {
        fprintf(stderr, "Erreur malloc name de scene : %s\n", name);
        return NULL;
    }
    strcpy(scene->name, name);


    // initialise les pointeurs sur fonctions 
    scene->load = load ;
    scene->unLoad = unLoad ;
    scene->handleEvents = handleEvents ;
    scene->update = update ;
    scene->render = render ;
    scene->data = NULL ;

    return scene;
}


/**
 * Creer et initialise le scene manager 
 */
SceneManager_t * create_scene_manager () {

    // alloue memoire de la structure 
    SceneManager_t * manager = malloc(sizeof(SceneManager_t)) ;
    
    // initialise les variables 
    manager->index = -1;
    manager->sceneCount = 0;

    return manager ;
}


/**
 * Libère la structure `SceneManager_t`
 */
void destroy_scene_manager (SceneManager_t ** manager) {
    
    if (existe(manager) && existe(*manager)) {

        for (int i = 0; i < (*manager)->sceneCount; i++) {
            destroy_scene(&(*manager)->scenes[i]);
        }

        free(*manager);
        *manager = NULL ;
    }
}


/**
 * Ajoute une scene dans la liste des scnenes du scene manager. Si la liste est complete ne produit 
 * aucun changement.
 */
void push_scene (SceneManager_t * manager, Scene_t * newScene) {

    // si la liste ne peut prendre de nouvelles scenes 
    if (manager->sceneCount == MAX_SCENES) {
        printf("[INFO] : Liste scenes complete, ne peut ajouter Scene \"%s\"\n", newScene->name);
        return ;
    }

    // ajoute la scene apres la derniere scene dans la liste, et incremente le compteur de scene 
    manager->scenes[manager->sceneCount] = newScene ;

    manager->sceneCount++;
}


/**
 * Ajoute une scene dans la liste d'attente (attribut char * nextScene du scene manager). 
 * Apres le rendu de la frame actuelle, la scene courante sera la scene en attente.
 */
void request_scene_change (SceneManager_t * manager, char * name) {
    strcpy(manager->nextScene, name) ;
}


/**
 * @brief Change la scène active dans le gestionnaire de scènes.
 * 
 * Cette fonction met à jour la scène actuelle en fonction de `manager->nextScene`. 
 * - Si `nextScene` correspond à une scène existante, elle remplace la scène actuelle.  
 * - Si aucune scène ne correspond, la fonction affiche un message d'erreur et retourne.  
 * - Si aucune scène n'était active (`index < 0`), elle charge simplement la nouvelle scène.  
 * 
 * @param manager Pointeur vers le gestionnaire de scènes `SceneManager_t` contenant les scènes.
 * 
 * @note `manager->nextScene` doit être une chaîne valide contenant le nom de la scène cible.
 * @warning Vérifie que la scène existe avant de tenter un changement, sinon un message d'erreur est affiché.
 */
void change_scene(SceneManager_t *manager) {
    if (manager == NULL) {
        fprintf(stderr, "Erreur: `manager` est NULL\n");
        return;
    }

    // Stocke le nom de la nouvelle scène en attente
    char newScene[128];
    strcpy(newScene, manager->nextScene);

    // Recherche de l'index de la nouvelle scène dans `manager->scenes`
    int newIndex = -1;
    for (int i = 0; i < MAX_SCENES; i++) {
        if (strcmp(manager->scenes[i]->name, newScene) == 0) {
            newIndex = i;
            printf("[INFO] : Nouvelle scène trouvée: index = %d\n", newIndex);
            break;  // On arrête la recherche dès qu'on trouve la scène
        }
    }

    // Vérifie si la scène a été trouvée
    if (newIndex == -1) {
        printf("[INFO] : Changement de scène impossible : \"%s\" introuvable\n", newScene);
        strcpy(manager->nextScene, "");
        return;
    }

    // Cas où aucune scène n'est actuellement jouée (`index < 0`)
    if (manager->index < 0) {
        Scene_t *scene = manager->scenes[newIndex];

        // Charge la nouvelle scène si elle a une fonction `load`
        if (scene && scene->load) {
            scene->load(scene);
        }

        // Met à jour l'index et réinitialise `nextScene`
        manager->index = newIndex;
        strcpy(manager->nextScene, "");

        printf("[INFO] : Changement de scène réussi: nouvelle scène = %s\n", newScene);

        return;
    }

    // Vérifie si la scène demandée est déjà la scène en cours
    if (strcmp(manager->scenes[manager->index]->name, newScene) == 0) {
        printf("[INFO] : Pas de changement de scène, \"%s\" déjà en cours\n", newScene);
        strcpy(manager->nextScene, "");
        return;  // Rien à changer, on sort de la fonction
    }

    // Stocke temporairement l'index et la scène actuelle
    int index = manager->index;
    Scene_t *scene = manager->scenes[index];

    // Décharge la scène actuelle si elle a une fonction `unLoad`
    if (scene && scene->unLoad) { 
        scene->unLoad(scene);
    }

    // Met à jour l'index vers la nouvelle scène
    manager->index = newIndex;
    index = newIndex;
    scene = manager->scenes[index];

    // Charge la nouvelle scène si elle a une fonction `load`
    if (scene && scene->load) {
        scene->load(scene);
    }

    // Réinitialise `nextScene`
    strcpy(manager->nextScene, "");

    printf("[INFO] : Changement de scène réussi: nouvelle scène = %s\n", newScene);
}


/**
 * Joue la scene actuelle en faisant appel a chacune des methodes. 
 * A la fin du rendu effectue le changement de scene si une scene est en attente (variable char * nextScene).
 * Renvoi 1 par défaut, 0 si la fin du jeu a été déclenché. 
 */
int play_scene (SceneManager_t * manager, SDL_Event * event) {

    // Récupère les données nécessaires de la scène actuelle
    int currentIndex = manager->index ;
    Scene_t * currentScene = manager->scenes[currentIndex] ;
    InfoScene_t * info = currentScene->data->get(currentScene->data, "info") ;
    
    int end = info->end ;
    info->currentTime = SDL_GetTicks() ;

    // efface l'affichage precedent
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    // Joue la scène
    currentScene->handleEvents(currentScene, event, manager) ;
    currentScene->update(currentScene, manager) ;
    currentScene->render(currentScene) ;

    // dessine le nouvel affichage 
    SDL_RenderPresent(renderer);

    // Vérification des scènes en attentes
    if (strcmp(manager->nextScene, "") != 0) {
        change_scene(manager); 
    }

    // Vérification de l'état du jeu (fin ou continuer)
    if (end == TRUE) {
        currentScene->unLoad(currentScene);
    } 

    return end ;
}


// ##########################################################################################
// ############################## ANCIENNES FONCTIONS DE SCENE ##############################
// ##########################################################################################


// void init_scene0(Camera_t * camera, Player_t * player, Map_t * map) {
//     init_elements_scene0(map);
    
// }


// void init_scene(Camera_t * camera, Player_t * player, Map_t * map) {
//     switch (gameStatus.scene) {
//         case 0 : 
//             init_scene0(camera, player, map) ;
//             break;
//         case 1 : 
//             init_scene0(camera, player, map) ;
//         default : 
//             break;
//     }
// }


// int play_scene0 (Camera_t * camera, Player_t * player, Map_t * map) {
//     SDL_Event event ;
//     uint32_t buttonClicked = FALSE;
//     uint8_t clicks = 0;
//     while (SDL_PollEvent(&event)) {
//         switch (event.type) {
//             case SDL_QUIT : 
//                 gameStatus.running = FALSE;
//                 break;
//             case SDL_KEYDOWN :
//                 switch (event.key.keysym.sym) {
//                     case SDLK_q :
//                         break;
//                     case SDLK_d :
//                         break;
//                     case SDLK_SPACE :
//                         break;
//                     case SDLK_BACKSPACE : 
//                         gameStatus.running = FALSE;
//                     default :
//                         break;
//                 }
//                 break;
//             case SDL_MOUSEBUTTONDOWN :
//                 printf("Bouton presser\n");
//                 element_update(map, &event);
//                 buttonClicked = 1;
//                 clicks = event.button.clicks;
//                 break;
//             case SDL_MOUSEBUTTONUP :
//                 printf("Bouton lever\n");
//                 element_update(map, &event);
//                 break;
//             case SDL_MOUSEMOTION : 
//                 move_element(map, &event);
//                 break;
//             default : 
//                 break;
//         }
//     }
     
//     // Effacer l'écran
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear (renderer); 

//     // position carre vert 
//     int cubeSize = 150;
//     SDL_Rect rect = {(camera->width / 2) - (cubeSize / 2), (camera->height / 2) - (cubeSize / 2), cubeSize, cubeSize};
    
//     // affiche carre vert en fonction de la position du curseur 
//     SDL_Point mouse ;
//     SDL_GetMouseState(&mouse.x, &mouse.y);
//     if (SDL_PointInRect(&mouse, &rect)) {
//         SDL_SetRenderDrawColor(renderer, 0, 200, 0, SDL_ALPHA_OPAQUE);
//         SDL_RenderFillRect(renderer, &rect);
        
//         // si boutton clicker sur carre vert change scene 
//         if (buttonClicked && clicks == 2) gameStatus.scene = 1;
//     }
//     else {
//         SDL_SetRenderDrawColor(renderer, 0, 100, 0, SDL_ALPHA_OPAQUE);
//         SDL_RenderFillRect(renderer, &rect);
//     }
    
    
//     // affiche le background test 
//     draw(camera, player, map);
    
//     // check la collision entre les element du bureau 
//     // check_elements_collision(map);
    
//     // Mettre à jour l'écran
//     SDL_RenderPresent (renderer);

//     // update position camera, se positionne sur un des 4 coins 
//     update_camera_random(camera);

//     // recupere les touche presser 
//     const uint8_t * keys = SDL_GetKeyboardState(NULL);
//     if (keys[SDL_SCANCODE_SPACE]) {
//         gameStatus.scene = 1;
//     }

//     return 0; 
// }


// int play_scene1 (Camera_t * camera, Player_t * player, Map_t * map) {

//     SDL_Event event ;
//     uint32_t buttonClicked = FALSE;
//     while (SDL_PollEvent(&event)) {
//         switch (event.type) {
//             case SDL_QUIT : 
//                 gameStatus.running = FALSE;
//                 break;
//             case SDL_KEYDOWN :
//                 switch (event.key.keysym.sym) {
//                     case SDLK_q :
//                         break;
//                     case SDLK_d :
//                         break;
//                     case SDLK_SPACE :
//                         break;
//                     case SDLK_BACKSPACE : 
//                         gameStatus.running = FALSE;
//                         break;
//                     case SDLK_f :
//                         camera->followPlayer = (camera->followPlayer + 1) % 2; printf("changement follow\n");
//                         break;
//                     case SDLK_i :
//                         printf("INFO :\nplayer x, y, vx, vy : %f %f %f %f\ncamera x, y : %f %f\n", player->body.position.x, player->body.position.y, player->body.vx, player->body.vy, camera->x, camera->y);
//                         break;
//                     default :
//                         break;
//                 }
//                 break;
//             case SDL_MOUSEBUTTONDOWN :
//                 buttonClicked = SDL_MOUSEBUTTONDOWN;
//                 break;
//             case SDL_MOUSEBUTTONUP :
//                 buttonClicked = SDL_MOUSEBUTTONUP;
//                 break;
//         }
//     }

//     const uint8_t * keys = SDL_GetKeyboardState(NULL);
//     handle_input(keys, player);

//     // Effacer l'écran
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear (renderer); 

//     // met a jour la position des objets dynamique 
//     update_player(player, &map->ground);
//     update_camera(camera, player);

//     // dessine le rendu 
//     if (draw (camera, player, map)) {
//         gameStatus.running = 0;
//     }

//     // Mettre à jour l'écran
//     SDL_RenderPresent (renderer);

//     gameStatus.updateCount++;

//     if (gameStatus.updateCount == 5) {
//         gameStatus.updateCount = 0;
//         update_player_anim_state (player);
//     }     
    
//     return 0;
// }
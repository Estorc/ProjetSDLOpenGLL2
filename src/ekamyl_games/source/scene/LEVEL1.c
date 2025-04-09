#include "../../include/lib.h"
#include "../../include/main.h"
#include "../../include/window.h"
#include "../../include/render.h"
#include "../../include/scene.h"
#include "../../include/text.h"
#include "../../include/dictionary.h"
#include "../../include/texture_loader.h"
#include "../../include/list.h" 
#include "../../include/event.h"
#include "../../include/player.h"
#include "../../include/map.h"
#include "../../include/camera.h"


// etat de la scène
#define STATE_PLAYING 0
#define STATE_DEAD 1
#define STATE_WIN 2
#define STATE_INTRO 3

// indice de la sprite sheet dans la listSpriteSheet
#define FLAME_BALL 0 

// indice des SDL_Texture dans la listSDL_Texture 
#define INDEX_BLACK_HEART 0 

// indice des text dans la listText 
#define INDEX_MSG_LOSS1 0
#define INDEX_MSG_LOSS2 1
#define INDEX_MSG_RETRY 2
#define INDEX_MSG_WIN 3
#define INDEX_MSG_INTRO0 4
#define INDEX_MSG_INTRO1 5

#define TIMER_MS 30000


// fonction de changement d'etat 
static void change_state (Scene_t * self, InfoScene_t * info) ;


// ensemble des evenements de la scene
static float new_projectile_trigger (Scene_t * scene, Event_t * event) ;
static void new_projectile (Scene_t * scene, float progress) ;

static float remaining_time_calculation_trigger (Scene_t * scene, Event_t * event) ;
static void remaining_time_calculation (Scene_t * scene, float progress) ;

static float change_visibility_win_msg_trigger (Scene_t * scene, Event_t * event) ;
static void change_visibility_win_msg (Scene_t * scene, float progress) ;


// fonctions propre a cette scene 
static void init_flame_ball_vertical (MapObj_t * object, List_t * listSpriteSheet, size_t idSpriteSheet) ;
static void init_map (Map_t * map) ;


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

    dict->set(dict, "info", info, free_cb);


    Player_t * player = player_constructor();
    if (player == NULL) {
        fprintf(stderr, "Erreur creation player\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "player", player, player_destructor_cb);


    Map_t * map = map_constructor();
    if (map == NULL) {
        fprintf(stderr, "Erreur creation map\n");
        destroy_dictionary(&self->data);
        return ;
    }
    init_map(map);
    dict->set(dict, "map", map, map_destructor_cb);


    Camera_t * camera = camera_constructor (player);
    if (camera == NULL) {
        fprintf(stderr, "Erreur creation camera\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "camera", camera, camera_destructor_cb);


    EventManager_t * eventManager = create_event_manager() ;
    if (!existe(eventManager)) {
        fprintf(stderr, "Erreur creation eventManager\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "eventManager", eventManager, destroy_event_manager_cb);


    List_t * listSDL_Texture = create_list(SDL_DestroyTexture_cb) ;
    if (!existe(listSDL_Texture)) {
        fprintf(stderr, "Erreur creation listSDL_Texture\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "listSDL_Texture", listSDL_Texture, destroy_list_cb);
    
    SDL_Texture * blackHeart = load_png("assets/ekamyl_games/assets/BlackHeart.png") ; 
    listSDL_Texture->stack(listSDL_Texture, blackHeart) ;


    List_t * listText = create_list(destroy_text_cb) ;
    if (listText == NULL) {
        fprintf(stderr, "Erreur creation listText\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "listText", listText, destroy_list_cb) ;

    List_t * listFont = create_list(TTF_CloseFont_cb) ;
    if (listFont == NULL) {
        fprintf(stderr, "Erreur creation listFont\n");
        destroy_dictionary(&self->data);
        return ;
    }
    dict->set(dict, "listFont", listFont, destroy_list_cb);

    TTF_Font * font0 = TTF_OpenFont("assets/ekamyl_games/assets/PressStart2P-Regular.ttf", 16) ;
    TTF_Font * font1 = TTF_OpenFont("assets/ekamyl_games/assets/PressStart2P-Regular.ttf", 20) ;
    TTF_Font * font2 = TTF_OpenFont("assets/ekamyl_games/assets/PressStart2P-Regular.ttf", 22) ;
    TTF_Font * font3 = TTF_OpenFont("assets/ekamyl_games/assets/PressStart2P-Regular.ttf", 32) ;
    listFont->stack(listFont, font0);
    listFont->stack(listFont, font1);
    listFont->stack(listFont, font2);
    listFont->stack(listFont, font3);

    load_texts_from_file("assets/ekamyl_games/data/textsLevel1.csv", listFont, listText);


    // timer qui va servir au compte a rebours du mini jeux 
    uint32_t * timer = malloc(sizeof(uint32_t)) ;
    *timer = TIMER_MS ;

    dict->set(dict, "timer", timer, free_cb);


    info->nextState = STATE_INTRO ;
    change_state(self, info);
    info->startTime = SDL_GetTicks() ;
    

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
    
    InfoScene_t * info = GET_INFO(self->data) ;
    Player_t * player = GET_PLAYER(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    uint32_t buttonClicked = FALSE;
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT : 
                info->end = TRUE ;
                break;
            case SDL_KEYDOWN :
                switch (event->key.keysym.sym) {
                    case SDLK_BACKSPACE : 
                        info->end = TRUE ;
                        break;
                    case SDLK_f :
                        camera->followPlayer = (camera->followPlayer + 1) % 2; printf("changement follow\n") ;
                        break;
                    case SDLK_i :
                        printf("INFO :\nplayer x, y, vx, vy : %f %f %f %f\ncamera x, y : %f %f\n", player->body.position.x, player->body.position.y, player->body.vx, player->body.vy, camera->x, camera->y);
                        break;
                    case SDLK_r : 
                        if (info->state == STATE_DEAD || info->state == STATE_WIN) {
                            request_scene_change(sceneManager, "DESKTOP");
                        }
                        break;
                    default :
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                buttonClicked = SDL_MOUSEBUTTONDOWN;
                break;
            case SDL_MOUSEBUTTONUP :
                buttonClicked = SDL_MOUSEBUTTONUP;
                break;
        }
    }


    if (info->state == STATE_PLAYING) {
        const uint8_t * keys = SDL_GetKeyboardState(NULL);
        handle_input(keys, player);
    }

    EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
    process_events(eventManager, self);

    return ; 
}


void LEVEL1_update (Scene_t * self, SceneManager_t * manager) {

    InfoScene_t * info = GET_INFO(self->data) ;
    List_t * listText ;
    List_t * listFont ;

    switch(info->state) {

    case STATE_INTRO :;
        
        listText = GET_LIST_TEXT(self->data) ;
        listFont = GET_LIST_FONT(self->data) ;

        Text_t * intro0 = listText->item(listText, INDEX_MSG_INTRO0) ;
        Text_t * intro1 = listText->item(listText, INDEX_MSG_INTRO1) ;

        if (intro0->hidden == TRUE) {
            intro0->hidden = FALSE ;
            intro1->hidden = FALSE ;
        }

        text_list_update_from_file(listText, listFont, "assets/ekamyl_games/data/textsLevel1.csv");

        if (info->currentTime >= info->startTime + 7000) {
            printf("pasage au state playing\n");
            info->nextState = STATE_PLAYING ;
            intro0->hidden = TRUE ;
            intro1->hidden = TRUE ;
        }
        
        break;

    case STATE_PLAYING :;

        Player_t * player = GET_PLAYER(self->data) ;
        Map_t * map = GET_MAP(self->data) ;
        Camera_t * camera = GET_CAMERA(self->data) ;    

        update_player(player, &map->ground);
        update_camera(camera, player);
        map_update(self, map, player);

        if ( (rand() % 30) == 0 ) {
            EventManager_t * eventManager = GET_EVENT_MANAGER(self->data) ;
            add_event(eventManager, 0, 0, new_projectile_trigger, new_projectile);
        }

        gameStatus.updateCount++;
        if (gameStatus.updateCount == 5) {
            gameStatus.updateCount = 0;
            update_player_anim_state (player);
        }   

        if (player->pv == 0) {
            info->nextState = STATE_DEAD ;
            printf("passaeg au state dead\n");
        }

        uint32_t * timer = GET_TIMER(self->data) ;
        if (*timer == 0) {
            info->nextState = STATE_WIN ;
            printf("passaeg au state win\n");
        }

        break; 
    
    case STATE_DEAD :;
        listText = GET_LIST_TEXT(self->data) ;
        listFont = GET_LIST_FONT(self->data) ;

        // demarre l'animation du text1 lorsque le text0 a terminer son animation
        Text_t * text1 = listText->item(listText, 1) ;
        if (text1->hidden == TRUE) {

            Text_t * text0 = listText->item(listText, 0) ;
            if (text0->animation.playing == FALSE) {
                text1->hidden = FALSE ;
            }
        }

        text_list_update_from_file(listText, listFont, "assets/ekamyl_games/data/textsLevel1.csv");


        Text_t * msgRetry = listText->item(listText, INDEX_MSG_RETRY) ;
        if (msgRetry->hidden == TRUE && info->currentTime - info->startTime >= 2000) {
            msgRetry->hidden = FALSE ;
        }

        break;

    case STATE_WIN :;
        listText = GET_LIST_TEXT(self->data) ;
        listFont = GET_LIST_FONT(self->data) ;

        icon_locked = FALSE ;

        text_list_update_from_file(listText, listFont, "assets/ekamyl_games/data/textsLevel1.csv");

        break;

    default :
        break;
    }


    // effectue le changement d'etat de la scene 
    if (info->nextState != info->state) {
        change_state(self, info);   
    }

    return ; 
}


void LEVEL1_render (Scene_t * self) {

    InfoScene_t * info = GET_INFO(self->data) ;

    Player_t * player = GET_PLAYER(self->data) ;
    Map_t * map = GET_MAP(self->data) ;
    Camera_t * camera = GET_CAMERA(self->data) ;

    // dessine le rendu 
    draw (camera, player, map);
    
    
    // affiche les pv restant 
    List_t * listSDL_Texture = GET_LIST_SDL_TEXTURE(self->data) ;
    draw_player_pv(player, listSDL_Texture->item(listSDL_Texture, INDEX_BLACK_HEART));

    // affiche le temps restant 
    List_t * listFont = GET_LIST_FONT(self->data) ;
    uint32_t * timer = GET_TIMER(self->data) ;
    draw_timer(*timer, 
        (SDL_Rect){WINDOW_WIDTH / 2 - 70, 20, 0, 0}, 
        listFont->item(listFont, 0),
        (SDL_Color){202, 202, 202, SDL_ALPHA_OPAQUE}
    );


    if (info->state == STATE_INTRO) {

        // assombri l'ecran 
        SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT} ;
        draw_rect_filled(rect, (SDL_Color){47, 47, 47, 255});

        List_t * listText = GET_LIST_TEXT(self->data) ;
        for (int i = 0; i < listText->size; i++) {
            draw_text(listText->item(listText, i));
        }
    }
    else if (info->state == STATE_DEAD) {
        
        // affiche un effet de glitch 
        apply_glitch(camera, map->background);

        // assombri l'ecran 
        SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT} ;
        draw_rect_filled(rect, (SDL_Color){0, 0, 0, 130});

        List_t * listText = GET_LIST_TEXT(self->data) ;
        for (int i = 0; i < listText->size; i++) {
            draw_text(listText->item(listText, i));
        }
    }
    else if (info->state == STATE_WIN) {
        
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

    case STATE_INTRO :;

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;

    case STATE_PLAYING :;

        add_event(eventManager, 1000, TIMER_MS, remaining_time_calculation_trigger, remaining_time_calculation) ;

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;

    case STATE_DEAD :;
        
        List_t * listText = GET_LIST_TEXT(self->data) ;
        
        Text_t * text = listText->item(listText, 0) ;
        text->hidden = FALSE ;

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;

    case STATE_WIN :;

        add_event(eventManager, 2000, 0, change_visibility_win_msg_trigger, change_visibility_win_msg);

        info->startTime = info->currentTime ;
        info->state = info->nextState ;
        break;
    
    default:
        break;
    }
}


static 
float new_projectile_trigger (Scene_t * scene, Event_t * event) {
    return 1.0f ;
}
static
void new_projectile (Scene_t * scene, float progress) {
    
    Map_t * map = GET_MAP(scene->data) ;

    MapObj_t * object = create_mapObj() ;
    if (object == NULL) {
        printf("Impossible d'ajouter une FLAME BALL, erreur creation object\n");
        return ;
    }

    init_flame_ball_vertical(object, map->listSpriteSheet, FLAME_BALL) ;
    map->listObjects->stack(map->listObjects, object);
}
static
float remaining_time_calculation_trigger (Scene_t * scene, Event_t * event) {

    InfoScene_t * info = GET_INFO(scene->data) ;
    
    if (info->currentTime >= event->execTime) {

        return (float)(info->currentTime - event->execTime) / (float)(event->duration) ;
    }

    return 0.0f ;
}
static
void remaining_time_calculation (Scene_t * scene, float progress) {

    InfoScene_t * info = GET_INFO(scene->data) ;

    if (info->state == STATE_PLAYING) {
        uint32_t * timer = scene->data->get(scene->data, "timer") ;
        *timer = TIMER_MS * (1 - progress) ;
    }
}
static
float change_visibility_win_msg_trigger (Scene_t * scene, Event_t * event) {
    
    InfoScene_t * info = GET_INFO(scene->data) ;
    
    if (info->currentTime >= event->execTime) {

        return 1.0f ;
    }

    return 0.0f ;
}
static
void change_visibility_win_msg (Scene_t * scene, float progress) {

    List_t * listText = GET_LIST_TEXT(scene->data) ;

    Text_t * msgWin = listText->item(listText, INDEX_MSG_WIN) ;
    msgWin->hidden = FALSE ;

    Text_t * msgRetry = listText->item(listText, INDEX_MSG_RETRY) ;
    msgRetry->hidden = FALSE ;
}





/**
 * Initialise une flame ball vertical dans une position aléatoire (sur l'axis X) en haut de la map.  
 */
static
void init_flame_ball_vertical (MapObj_t * object, List_t * listSpriteSheet, size_t idSpriteSheet) {
    
    if (!existe(object)) {
        printf("impossible d'init l'object car NULL\n");
        return ;
    }

    object->sprite.texture = listSpriteSheet->item(listSpriteSheet, idSpriteSheet) ;
    object->sprite.animationSpeed = 5 ;
    object->sprite.currentFrame = 0 ;
    object->sprite.frameCount = 0 ;
    object->sprite.hidden = FALSE ;
    object->sprite.loop = TRUE ;
    object->sprite.numFrames = 6 ;
    object->sprite.playing = TRUE ;
    object->sprite.position = (SDL_Rect){ (BACKGROUND_WIDTH / 20) * (rand() % 20) , 0 , 25 , 189 } ;
    object->sprite.srcrect = (SDL_Rect){0, 0, 25, 189} ;
    object->sprite.typeAnim = DEFAULT ;

    object->vx = 0 ;
    object->vy = 3 ;

    object->idAction = ACT_ID_LOSS_PV ;
    object->idSpriteSheet = idSpriteSheet ;
}


static
void init_map (Map_t * map) {

    if (existe(map)) {

        map->background = load_png("assets/ekamyl_games/assets/background_grey.png");
        if (map->background == NULL) {
            printf("Erreur background dans init map level1\n");
            return ;
        }

        map->ground = (SDL_FRect){0, BACKGROUND_HEIGHT - GROUND_HEIGHT, GROUND_WIDTH, GROUND_HEIGHT} ;

        SDL_Texture * spriteSheet1 = load_png("assets/ekamyl_games/assets/projectileVertical.png") ;
        if (spriteSheet1 == NULL) {
            printf("Erreur spriteSheet dans init map level1\n");
            return ;
        }
        map->listSpriteSheet->stack(map->listSpriteSheet, spriteSheet1);
    }
}
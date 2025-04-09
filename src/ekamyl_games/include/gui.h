#pragma once 


#include "lib.h"

#define CLOSE_BUTTON 0

typedef struct Desktop_u Desktop_t ;
typedef struct Window_u Window_t ;
typedef struct List_u List_t ;
typedef struct Text_u Text_t ;
typedef struct Scene_u Scene_t ;
typedef struct SceneManager_u SceneManager_t ; 
extern SceneManager_t * sceneManager ;

typedef enum {
    WIDGET_BUTTON,
    WIDGET_TEXT,
    WIDGET_ICON
} WidgetType_t ;

typedef struct Icon_u {
    SDL_Rect srcrect ;

    uint8_t isClicked ;
    uint8_t isDragged ;

    uint8_t actionID ;
} Icon_t ;

typedef struct Button_u {
    SDL_Rect srcrect ;

    uint8_t actionID ;
} Button_t ;

typedef struct TextBox_u {
    SDL_Texture * texture ;
    char * string ;
} TextBox_t ;

typedef struct {
    SDL_Rect relPosition ;          // Position relative a la fenetre et taille (x, y, w, h)
    WidgetType_t type;              // Type du widget (bouton, textbox ou icon)
    union {
        Icon_t icon ;
        Button_t button ;
        TextBox_t text ;
    };
} Widget_t ;

typedef struct WinTheme_u {
    TTF_Font * font ;

    SDL_Color textColor ;
    SDL_Color bgColor ;
} WinTheme_t ;

typedef struct Window_u {
    SDL_Texture * background ;
    SDL_Rect position ;

    SDL_Texture * spriteSheet ;
    Widget_t * tabWidget ;       // Liste des widgets 
    int widgetCount ;         // Nombre actuel de widgets

    WinTheme_t theme ;

    uint8_t isActive ;
    uint8_t isDragged ;

    uint8_t id ;
} Window_t ;

typedef struct Desktop_u {
    List_t * listWindow ;
    Window_t mainWindow ;
} Desktop_t ;


Desktop_t * create_desktop () ;
void destroy_desktop(Desktop_t **desktop) ;
void destroy_desktop_cb (void * desktop) ;

Window_t * create_window () ;
void destroy_window (Window_t ** window) ;
void destroy_window_cb (void * window) ;
void load_windows_from_file (List_t * list, char * dataPath, WinTheme_t theme) ;
void window_change_theme (Window_t * window, WinTheme_t newTheme) ;
void window_add_widget (Window_t * window, Widget_t newWidget) ;

int init_widgets_from_file (Widget_t * tab, char * dataPath, WinTheme_t theme) ;

int desktop_element_update (Desktop_t * desktop, SDL_Event * event) ;
void desktop_move_element (Desktop_t * desktop, SDL_Event * event) ;
Window_t * desktop_get_window_from_id (Desktop_t * desktop, uint8_t id) ;
void desktop_handle_button_events (Scene_t * scene, uint8_t actionID) ;
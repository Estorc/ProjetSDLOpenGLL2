#pragma once 


#include "lib.h"

typedef struct Desktop_u Desktop_t ;
typedef struct Window_u Window_t ;
typedef struct List_u List_t ;
typedef struct Text_u Text_t ;

typedef enum {
    WIDGET_BUTTON,
    WIDGET_TEXT,
    WIDGET_ICON
} WidgetType_t ;

typedef struct Icon_u {
    SDL_Rect srcrect ;

    uint8_t isClicked ;
    uint8_t isDragged ;

    void (*onClick)(Desktop_t *) ;
} Icon_t ;

typedef struct Button_u {
    SDL_Rect srcrect ;

    uint8_t isClicked ;

    void (*onClick)(Window_t *) ;
} Button_t ;

typedef struct TextBox_u {
    SDL_Rect rect ;
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
void init_desktop_main_window (Window_t * window, WinTheme_t theme) ;
void load_windows_from_file (List_t * list, char * dataPath, WinTheme_t theme) ;
void window_change_theme (Window_t * window, WinTheme_t newTheme) ;

void init_widgets_from_file (Widget_t * tab, char * dataPath, WinTheme_t theme) ;

int element_update(Desktop_t * desktop, SDL_Event * event) ;
void move_element (Desktop_t * desktop, SDL_Event * event) ;
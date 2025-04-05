#pragma once 


#include "lib.h"

typedef struct Window_u Window_t ;

typedef enum {
    WIDGET_BUTTON,
    WIDGET_LABEL
} WidgetType_t ;

typedef struct {
    WidgetType_t type;          // Type du widget (bouton, label, textbox, etc.)
    SDL_Rect rect;            // Position et taille (x, y, w, h)
    char label[256];           // Texte affiché (si applicable)
    bool isFocused;          // Si le widget est actif (pour saisir du texte par ex.)
    void (*on_click)();       // Fonction appelée si cliqué (callback)
} Widget_t ;

typedef struct {
    Window_t * parentWindow ;
    Widget * widgets;       // Liste des widgets (32 max ici)
    int widgetCount;         // Nombre actuel de widgets
} GUI_t ;

typedef struct Window_u {
    GUI_t gui ;
    SDL_Texture * background ;
    SDL_Rect position ;
    SDL_Rect dstrect ;
    
} Window_t ;

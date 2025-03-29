#pragma once 


#include "lib.h"


typedef enum {
    WIDGET_BUTTON,
    WIDGET_LABEL,
    WIDGET_TEXTBOX
} WidgetType;

typedef struct {
    WidgetType type;          // Type du widget (bouton, label, textbox, etc.)
    SDL_Rect rect;            // Position et taille (x, y, w, h)
    char label[512];           // Texte affiché (si applicable)
    bool isFocused;          // Si le widget est actif (pour saisir du texte par ex.)
    void (*on_click)();       // Fonction appelée si cliqué (callback)
} Widget;

typedef struct {
    Widget * widgets;       // Liste des widgets (32 max ici)
    int widgetCount;         // Nombre actuel de widgets
} GUI;

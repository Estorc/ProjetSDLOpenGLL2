#ifndef SETTINGS_H
#define SETTINGS_H

typedef enum Resolutions {
    RES_RESPONSIVE,
    RES_NATIVE,
    RES_640x480,
    RES_800x600,
    RES_1024x768,
    RES_1280x720,
    RES_1280x800,
    RES_1280x1024,
    RES_1366x768,
    RES_1440x900,
    RES_1600x900,
    RES_1680x1050,
    RES_1920x1080,
    RES_1920x1200,
    RES_2560x1440,
    RES_COUNT,
} Resolutions;

typedef struct Settings {
    bool show_fps;
    bool cast_shadows;
    bool window_fullscreen;
    u16 resolution;
} Settings;

void get_resolution(int *width, int *height);
extern const char resolutionsText[RES_COUNT][256];
extern Settings settings;
#endif
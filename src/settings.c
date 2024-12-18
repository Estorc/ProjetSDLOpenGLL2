#include "types.h"
#include "settings.h"
#include <SDL2/SDL.h>
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "window.h"

const char resolutionsText[RES_COUNT][256] = {
    "Responsive",
    "Native",
    "640x480",
    "800x600",
    "1024x768",
    "1280x720",
    "1280x800",
    "1280x1024",
    "1366x768",
    "1440x900",
    "1600x900",
    "1680x1050",
    "1920x1080",
    "1920x1200",
    "2560x1440"
};


void get_resolution(int *width, int *height) {
    switch (settings.resolution) {
        case RES_RESPONSIVE:
            SDL_GetWindowSize(window.sdl_window, width, height);
            break;
        case RES_NATIVE:
            SDL_Rect rect;
            SDL_GetDisplayBounds(0, &rect);
            *width = rect.w;
            *height = rect.h;
            break;
        case RES_640x480:
            *width = 640;
            *height = 480;
            break;
        case RES_800x600:
            *width = 800;
            *height = 600;
            break;
        case RES_1024x768:
            *width = 1024;
            *height = 768;
            break;
        case RES_1280x720:
            *width = 1280;
            *height = 720;
            break;
        case RES_1280x800:
            *width = 1280;
            *height = 800;
            break;
        case RES_1280x1024:
            *width = 1280;
            *height = 1024;
            break;
        case RES_1366x768:
            *width = 1366;
            *height = 768;
            break;
        case RES_1440x900:
            *width = 1440;
            *height = 900;
            break;
        case RES_1600x900:
            *width = 1600;
            *height = 900;
            break;
        case RES_1680x1050:
            *width = 1680;
            *height = 1050;
            break;
        case RES_1920x1080:
            *width = 1920;
            *height = 1080;
            break;
        case RES_1920x1200:
            *width = 1920;
            *height = 1200;
            break;
        case RES_2560x1440:
            *width = 2560;
            *height = 1440;
            break;
        default:
            *width = 1280;
            *height = 720;
            break;
    }
}
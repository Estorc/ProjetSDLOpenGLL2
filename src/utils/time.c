#include <time.h>
#include <SDL2/SDL.h>

float get_time_in_seconds() {
    return SDL_GetTicks64()/1000.0f;
}
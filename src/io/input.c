#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>
#include <render/render.h>
#include <window.h>
#include <render/camera.h>
#include <io/input.h>
#include <settings.h>

void default_input_settings() {
    Game.settings->keybinds.up = SDLK_z;
    Game.settings->keybinds.down = SDLK_s;
    Game.settings->keybinds.left = SDLK_q;
    Game.settings->keybinds.right = SDLK_d;

    Game.settings->keybinds.jump = SDLK_SPACE;
    Game.settings->keybinds.crouch = SDLK_LCTRL;
    Game.settings->keybinds.sprint = SDLK_LSHIFT;

    Game.settings->keybinds.menu = SDLK_ESCAPE;
    Game.settings->keybinds.flashlight = SDLK_f;
    Game.settings->keybinds.validate = SDLK_RETURN;
    Game.settings->keybinds.interact = SDLK_e;
    Game.settings->keybinds.fullscreen = SDLK_F11;
}

void init_input(Input *input) {
    input->active_keys = 0;
    input->released_keys = 0;
    input->pressed_keys = 0;
    input->mouse.x = 0;
    input->mouse.y = 0;

    input->mouse.dx = 0;
    input->mouse.dy = 0;

    input->mouse.pressed_button = 0;
    input->mouse.released_button = 0;
    input->mouse.active_button = 0;
    strcpy(input->inputBuffer, "");

    input->locked = false;
}


int update_input(Input *input) {
    input->mouse.dx = 0;
    input->mouse.dy = 0;
    input->mouse.scroll_x = 0;
    input->mouse.scroll_y = 0;
    input->mouse.lastX = input->mouse.x;
    input->mouse.lastY = input->mouse.y;
    input->released_keys = 0;
    input->pressed_keys = 0;
    input->mouse.pressed_button = 0;
    input->mouse.released_button = 0;
    input->text_input = false;
    if (input->locked) return 0;
 	SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
					case SDLK_BACKSPACE:
                        remove_last_utf8_char(input->inputBuffer);
                        input->text_input = true;
                        break;
                    default:
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.left, KEY_LEFT);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.right, KEY_RIGHT);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.up, KEY_UP);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.down, KEY_DOWN);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.jump, KEY_JUMP);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.sprint, KEY_SPRINT);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.crouch, KEY_CROUCH);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.menu, KEY_MENU);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.flashlight, KEY_FLASHLIGHT);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.interact, KEY_INTERACT);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.fullscreen, KEY_FULLSCREEN);
                        HANDLE_KEY_PRESSED(Game.settings->keybinds.validate, KEY_VALIDATE);
                        break;
                }
                break;
            case SDL_KEYUP:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
                    default:
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.left, KEY_LEFT);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.right, KEY_RIGHT);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.up, KEY_UP);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.down, KEY_DOWN);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.jump, KEY_JUMP);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.sprint, KEY_SPRINT);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.crouch, KEY_CROUCH);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.menu, KEY_MENU);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.flashlight, KEY_FLASHLIGHT);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.interact, KEY_INTERACT);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.fullscreen, KEY_FULLSCREEN);
                        HANDLE_KEY_RELEASED(Game.settings->keybinds.validate, KEY_VALIDATE);
                        break;
                }
                break;
            case SDL_TEXTINPUT:
                if (strlen(input->inputBuffer)+1 < 100)
                    strcat(input->inputBuffer, event.text.text);
                input->text_input = true;
                break;
            case SDL_MOUSEMOTION:
                if (input->mouse.x == 0 && input->mouse.y == 0) {
                    input->mouse.lastX = event.motion.x;
                    input->mouse.lastY = event.motion.y;
                }
                int window_width, window_height;
                SDL_GetWindowSize(Game.window->sdl_window, &window_width, &window_height);
                int res_width, res_height;
                get_resolution(&res_width, &res_height);

				input->mouse.x = event.motion.x / (float)window_width * res_width;
				input->mouse.y = event.motion.y / (float)window_height * res_height;

                input->mouse.dx = event.motion.xrel;
                input->mouse.dy = event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                input->mouse.pressed_button = event.button.button;
                input->mouse.active_button |= event.button.button;
                break;
            case SDL_MOUSEBUTTONUP:
                input->mouse.released_button = event.button.button;
                input->mouse.active_button &= ~event.button.button;
                break;
            case SDL_MOUSEWHEEL:
                if (event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) {
                    input->mouse.scroll_x = event.wheel.x;
                    input->mouse.scroll_y = event.wheel.y;
                } else if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                    input->mouse.scroll_x = event.wheel.x;
                    input->mouse.scroll_y = event.wheel.y;
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    return 1;
                }
                break;
            case SDL_QUIT:
                return -1;
                break;
        }
    }

	return 0;

}
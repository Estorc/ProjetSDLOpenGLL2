#include "../types.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include "../math/math_util.h"
#include "model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../render/render.h"
#include "../window.h"
#include "../render/camera.h"
#include "input.h"



void init_input(Input *input) {
    input->active_keys = 0;
    input->released_keys = 0;
    input->pressed_keys = 0;
    input->mouse.x = 0;
    input->mouse.y = 0;
    input->mouse.pressed_button = 0;
    input->mouse.released_button = 0;
    input->mouse.active_button = 0;
    strcpy(input->inputBuffer, "");
}


int update_input(Input *input) {
    input->mouse.scroll_x = 0;
    input->mouse.scroll_y = 0;
    input->mouse.lastX = input->mouse.x;
    input->mouse.lastY = input->mouse.y;
    input->released_keys = 0;
    input->pressed_keys = 0;
    input->mouse.pressed_button = 0;
    input->mouse.released_button = 0;
    input->text_input = false;
	SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
					case SDLK_q:
                    case SDLK_LEFT:
                        HANDLE_KEY_PRESSED(KEY_LEFT);
                        break;
					case SDLK_d:
                    case SDLK_RIGHT:
                        HANDLE_KEY_PRESSED(KEY_RIGHT);
                        break;
					case SDLK_z:
                    case SDLK_UP:
						HANDLE_KEY_PRESSED(KEY_UP);
                        break;
					case SDLK_s:
                    case SDLK_DOWN:
                        HANDLE_KEY_PRESSED(KEY_DOWN);
                        break;
                    case SDLK_SPACE:
                        HANDLE_KEY_PRESSED(KEY_JUMP);
                        break;
                    case SDLK_LSHIFT:
                        HANDLE_KEY_PRESSED(KEY_SHIFT);
                        break;
                    case SDLK_LCTRL:
                        HANDLE_KEY_PRESSED(KEY_CROUCH);
                        break;
                    case SDLK_ESCAPE:
						HANDLE_KEY_PRESSED(KEY_MENU);
                        break;

					case SDLK_f:
						HANDLE_KEY_PRESSED(KEY_F);
                        break;
					case SDLK_g:
						HANDLE_KEY_PRESSED(KEY_G);
                        break;

					case SDLK_RETURN:
						HANDLE_KEY_PRESSED(KEY_ENTER);
                        break;
					case SDLK_BACKSPACE:
                        remove_last_utf8_char(input->inputBuffer);
                        input->text_input = true;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
					case SDLK_q:
                    case SDLK_LEFT:
                        HANDLE_KEY_RELEASED(KEY_LEFT);
                        break;
					case SDLK_d:
                    case SDLK_RIGHT:
                        HANDLE_KEY_RELEASED(KEY_RIGHT);
                        break;
					case SDLK_z:
                    case SDLK_UP:
						HANDLE_KEY_RELEASED(KEY_UP);
						break;
					case SDLK_s:
                    case SDLK_DOWN:
                        HANDLE_KEY_RELEASED(KEY_DOWN);
                        break;
                    case SDLK_SPACE:
                        HANDLE_KEY_RELEASED(KEY_JUMP);
                        break;
                    case SDLK_LSHIFT:
                        HANDLE_KEY_RELEASED(KEY_SHIFT);
                        break;
                    case SDLK_LCTRL:
                        HANDLE_KEY_RELEASED(KEY_CROUCH);
                        break;
                    case SDLK_ESCAPE:
						HANDLE_KEY_RELEASED(KEY_MENU);
                        break;

					case SDLK_f:
						HANDLE_KEY_RELEASED(KEY_F);
                        break;
					case SDLK_g:
						HANDLE_KEY_RELEASED(KEY_G);
                        break;

					case SDLK_RETURN:
						HANDLE_KEY_RELEASED(KEY_ENTER);
                        break;
                    default:
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
                SDL_GetWindowSize(window.sdl_window, &window_width, &window_height);
                int res_width, res_height;
                get_resolution(&res_width, &res_height);

				input->mouse.x = event.motion.x / (float)window_width * res_width;
				input->mouse.y = event.motion.y / (float)window_height * res_height;
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
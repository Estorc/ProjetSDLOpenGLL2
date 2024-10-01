#ifndef INPUT_H
#define INPUT_H

#define HANDLE_KEY_PRESSED(key)     if (!(input->active_keys & key)) input->pressed_keys |= key; \
                                    input->active_keys |= key;

#define HANDLE_KEY_RELEASED(key)    if ((input->active_keys & key)) input->released_keys |= key; \
                                    input->active_keys &= ~key;

enum Keys {
    KEY_UP      = 1 << 0,
    KEY_RIGHT   = 1 << 1,
    KEY_DOWN    = 1 << 2,
    KEY_LEFT    = 1 << 3,
    KEY_JUMP    = 1 << 4,
    KEY_CROUCH  = 1 << 5,
    KEY_MENU    = 1 << 6,
    KEY_SHIFT   = 1 << 7,
    KEY_F       = 1 << 8,
    KEY_G       = 1 << 9,
    KEY_ENTER   = 1 << 10,
    KEY_COUNT,
};

typedef struct Mouse {
    s32 x, y, lastX, lastY;
    u8 pressed_button, active_button, released_button;
} Mouse;

typedef struct Input {
    u16 active_keys;
    u16 pressed_keys;
    u16 released_keys;
    char inputBuffer[100];
    Mouse mouse;
} Input;

void init_input(Input *input);
int update_input(Input *input);
#endif
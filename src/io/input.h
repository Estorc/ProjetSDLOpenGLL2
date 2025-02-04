#ifndef INPUT_H
#define INPUT_H

/**
 * @defgroup InputHandling Input Handling
 * @brief Functions and structures for handling user input.
 * 
 * This module provides macros, enums, and structures for managing user input, 
 * including keyboard and mouse events. It allows for tracking key presses, 
 * releases, and active states, as well as mouse movements and button states.
 * 
 * @{
 */

/**
 * @brief Macro to handle key press events.
 * 
 * This macro checks if a key is not currently active and, if so, marks it as pressed 
 * and active in the input structure.
 * 
 * @param key The key to be handled.
 */

#define HANDLE_KEY_PRESSED(key)     if (!(input->active_keys & key)) input->pressed_keys |= key; \
                                    input->active_keys |= key;

/**
 * @brief Macro to handle key release events.
 * 
 * This macro checks if a key is currently active and, if so, marks it as released 
 * and removes it from the active keys in the input structure.
 * 
 * @param key The key to be handled.
 */

#define HANDLE_KEY_RELEASED(key)    if ((input->active_keys & key)) input->released_keys |= key; \
                                    input->active_keys &= ~key;

/**
 * @enum Keys
 * @brief Enumeration of possible keys.
 * 
 * This enum defines bitwise values for various keys that can be pressed or released. 
 * Each key is represented by a unique bit in a 16-bit integer.
 */

enum Keys {
    KEY_UP      = 1 << 0, /**< The up arrow key. */
    KEY_RIGHT   = 1 << 1, /**< The right arrow key. */
    KEY_DOWN    = 1 << 2, /**< The down arrow key. */
    KEY_LEFT    = 1 << 3, /**< The left arrow key. */
    KEY_JUMP    = 1 << 4, /**< The jump key. */
    KEY_CROUCH  = 1 << 5, /**< The crouch key. */
    KEY_MENU    = 1 << 6, /**< The menu key. */
    KEY_SHIFT   = 1 << 7, /**< The shift key. */
    KEY_F       = 1 << 8, /**< The F key. */
    KEY_G       = 1 << 9, /**< The G key. */
    KEY_ENTER   = 1 << 10, /**< The enter key. */
    KEY_COUNT, /**< The total number of keys. */
};

/**
 * @struct Mouse
 * @brief Structure to store mouse state.
 * 
 * This structure holds the current and last positions of the mouse, 
 * scroll wheel movements, and the states of mouse buttons.
 */

typedef struct Mouse {
    s32 x; /**< The current x-coordinate of the mouse. */
    s32 y; /**< The current y-coordinate of the mouse. */
    s32 lastX; /**< The last x-coordinate of the mouse. */
    s32 lastY; /**< The last y-coordinate of the mouse. */

    s8 scroll_x; /**< The horizontal scroll movement of the mouse. */
    s8 scroll_y; /**< The vertical scroll movement of the mouse. */

    u8 pressed_button; /**< Bitfield representing mouse buttons that were pressed. */
    u8 active_button; /**< Bitfield representing currently active mouse buttons. */
    u8 released_button; /**< Bitfield representing mouse buttons that were released. */
} Mouse;

/**
 * @struct Input
 * @brief Structure to store input state.
 * 
 * This structure holds the states of keys and mouse, as well as text input buffer.
 */

typedef struct Input {
    u16 active_keys; /**< Bitfield representing currently active keys. */
    u16 pressed_keys; /**< Bitfield representing keys that were pressed. */
    u16 released_keys; /**< Bitfield representing keys that were released. */
    bool text_input; /**< Flag indicating text input. */
    char inputBuffer[100]; /**< Buffer for text input. */
    Mouse mouse; /**< Mouse state. */
} Input;

/**
 * @brief Initializes the input structure.
 * 
 * This function sets the initial state of the input structure, 
 * clearing all key states and initializing the mouse state.
 * 
 * @param input Pointer to the Input structure to be initialized.
 */
void init_input(Input *input);

/**
 * @brief Updates the input state.
 * 
 * This function processes keyboard and mouse events to update the input structure. 
 * It handles key presses and releases, mouse movements, and button states.
 * 
 * @param input Pointer to the Input structure that stores current input states.
 * @return Returns -1 if the escape key is pressed (to indicate an exit), otherwise returns 0.
 */
int update_input(Input *input);

/**
 * @brief External declaration of the Input structure.
 */
extern Input input;

/** @} */

#endif
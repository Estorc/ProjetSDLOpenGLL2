#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @file settings.h
 * @brief Defines the available screen resolutions for the application.
 */

/**
 * @enum Resolutions
 * @brief Enumeration of possible screen resolutions.
 *
 * This enumeration defines various screen resolutions that the application can support.
 * It includes both standard resolutions and a responsive option.
 *
 * @var RES_RESPONSIVE
 * The application will adjust to the screen size dynamically.
 *
 * @var RES_NATIVE
 * The application will use the native resolution of the display.
 *
 * @var RES_640x480
 * 640x480 resolution.
 *
 * @var RES_800x600
 * 800x600 resolution.
 *
 * @var RES_1024x768
 * 1024x768 resolution.
 *
 * @var RES_1280x720
 * 1280x720 resolution (HD).
 *
 * @var RES_1280x800
 * 1280x800 resolution.
 *
 * @var RES_1280x1024
 * 1280x1024 resolution.
 *
 * @var RES_1366x768
 * 1366x768 resolution.
 *
 * @var RES_1440x900
 * 1440x900 resolution.
 *
 * @var RES_1600x900
 * 1600x900 resolution.
 *
 * @var RES_1680x1050
 * 1680x1050 resolution.
 *
 * @var RES_1920x1080
 * 1920x1080 resolution (Full HD).
 *
 * @var RES_1920x1200
 * 1920x1200 resolution.
 *
 * @var RES_2560x1440
 * 2560x1440 resolution (Quad HD).
 *
 * @var RES_COUNT
 * The total number of resolutions defined in this enumeration.
 */
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

/**
 * @struct Keybinds
 * @brief A structure to hold key bindings for various actions in the game.
 *
 * This structure contains integer values representing the key codes for
 * different actions that can be performed in the game. Each member of the
 * structure corresponds to a specific action.
 *
 * @var Keybinds::up
 * The key code for moving up.
 *
 * @var Keybinds::down
 * The key code for moving down.
 *
 * @var Keybinds::left
 * The key code for moving left.
 *
 * @var Keybinds::right
 * The key code for moving right.
 *
 * @var Keybinds::jump
 * The key code for jumping.
 *
 * @var Keybinds::crouch
 * The key code for crouching.
 *
 * @var Keybinds::sprint
 * The key code for sprinting.
 *
 * @var Keybinds::menu
 * The key code for opening the menu.
 *
 * @var Keybinds::flashlight
 * The key code for toggling the flashlight.
 *
 * @var Keybinds::validate
 * The key code for validating an action.
 *
 * @var Keybinds::interact
 * The key code for interacting with objects.
 *
 * @var Keybinds::fullscreen
 * The key code for toggling fullscreen mode.
 */
typedef struct Keybinds {
    int up;
    int down;
    int left;
    int right;

    int jump;
    int crouch;
    int sprint;

    int menu;
    int flashlight;
    int validate;
    int interact;
    int fullscreen;
} Keybinds;

/**
 * @struct Settings
 * @brief A structure to hold various settings for the application.
 * 
 * This structure contains various settings that can be configured for the application,
 * including display options, audio settings, and keybindings.
 * 
 * @var Settings::show_fps
 * A boolean flag to indicate whether the FPS (frames per second) should be displayed.
 * 
 * @var Settings::cast_shadows
 * A boolean flag to indicate whether shadows should be cast in the application.
 * 
 * @var Settings::window_fullscreen
 * A boolean flag to indicate whether the application window should be in fullscreen mode.
 * 
 * @var Settings::resolution
 * An unsigned 16-bit integer representing the resolution setting of the application.
 * 
 * @var Settings::show_collision_boxes
 * A boolean flag to indicate whether collision boxes should be displayed.
 * 
 * @var Settings::master_volume
 * A float representing the master volume level of the application.
 * 
 * @var Settings::music_volume
 * A float representing the volume level for music in the application.
 * 
 * @var Settings::sfx_volume
 * A float representing the volume level for sound effects in the application.
 * 
 * @var Settings::fov
 * A float representing the field of view (FOV) setting of the application.
 * 
 * @var Settings::shadow_resolution
 * An integer representing the resolution of shadows in the application.
 * 
 * @var Settings::shadow_quality
 * An integer representing the quality level of shadows in the application.
 * 
 * @var Settings::texture_quality
 * An integer representing the quality level of textures in the application.
 * 
 * @var Settings::antialiasing
 * A boolean flag to indicate whether antialiasing is enabled.
 * 
 * @var Settings::vsync
 * A boolean flag to indicate whether vertical synchronization (VSync) is enabled.
 * 
 * @var Settings::keybinds
 * A Keybinds structure containing the keybindings for the application.
 */
typedef struct Settings {
    bool show_fps;
    bool cast_shadows;
    bool window_fullscreen;
    u16 resolution;
    bool show_collision_boxes;
    float master_volume;
    float music_volume;
    float sfx_volume;
    float fov;
    int shadow_resolution;
    int shadow_quality;
    int texture_quality;
    bool antialiasing;
    bool vsync;
    Keybinds keybinds;
} Settings;

/**
 * @brief Retrieves the current screen resolution.
 *
 * This function populates the provided width and height pointers with the
 * current screen resolution values.
 *
 * @param[out] width Pointer to an integer where the screen width will be stored.
 * @param[out] height Pointer to an integer where the screen height will be stored.
 */
void get_resolution(int *width, int *height);

/**
 * @var resolutionsText
 * @brief Array of resolution text strings.
 *
 * This external variable holds an array of resolution text strings, where each string
 * represents a different resolution option available in the application.
 * 
 * @note The size of each string is limited to 256 characters.
 * 
 * @warning Ensure that RES_COUNT is defined and corresponds to the number of resolution
 * options available.
 */
extern const char resolutionsText[RES_COUNT][256];

/**
 * @brief Saves the application settings to a configuration file.
 */
void save_settings();

/**
 * @brief Loads the application settings from a configuration file.
 */
void load_settings();
#endif
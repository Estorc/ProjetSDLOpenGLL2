#pragma once
/**
 * @file frame.h
 * @brief GUI Frame and related structures for SDL and OpenGL project.
 * 
 * This file contains the definitions of various structures used to create and manage
 * GUI frames, buttons, labels, input areas, and other elements in an SDL and OpenGL project.
 * 
 * @{
 */

/**
 * @enum ButtonState
 * @brief Represents the state of a button.
 * 
 * This enumeration defines the possible states a button can be in.
 */
typedef enum ButtonState {
    BUTTON_STATE_NORMAL,   /**< Button is in its normal state. */
    BUTTON_STATE_HOVERED,  /**< Button is being hovered over by the cursor. */
    BUTTON_STATE_PRESSED,  /**< Button is being pressed. */
} ButtonState;

/**
 * @struct ImageFrame
 * @brief Represents an image frame.
 * 
 * This structure holds the path to an image file.
 */
typedef struct ImageFrame {
    char path[256]; /**< Path to the image file. */
} ImageFrame;

/**
 * @struct RadioButton
 * @brief Represents a radio button.
 * 
 * This structure holds information about a radio button, including its checked state and ID.
 */
typedef struct RadioButton {
    u16 *checked; /**< Pointer to the checked state of the radio button. */
    u16 id;       /**< ID of the radio button. */
} RadioButton;

/**
 * @struct Button
 * @brief Represents a button.
 * 
 * This structure holds information about a button, including its state and whether it is checked.
 * It can also represent a radio button.
 */
typedef struct Button {
    bool *checked; /**< Pointer to the checked state of the button. */
    union {
        RadioButton *radiobutton; /**< Pointer to a RadioButton structure if the button is a radio button. */
    };
    ButtonState state; /**< Current state of the button. */
} Button;

/**
 * @struct SelectList
 * @brief Represents a selectable list.
 * 
 * This structure holds information about a selectable list, including its state, options, and selected item.
 */
typedef struct SelectList {
    ButtonState state; /**< Current state of the select list. */
    char (*options)[256]; /**< Array of options in the select list. */
    u16 *selected; /**< Pointer to the selected option. */
    u16 count; /**< Number of options in the select list. */
} SelectList;

/**
 * @struct Label
 * @brief Represents a label.
 * 
 * This structure holds the text of a label.
 */
typedef struct Label {
    char *text; /**< Text of the label. */
    int displayLength; /**< Number of characters to display. */
} Label;

/**
 * @struct InputArea
 * @brief Represents an input area.
 * 
 * This structure holds information about an input area, including its state, text, and default text.
 */
typedef struct InputArea {
    ButtonState state; /**< Current state of the input area. */
    char text[256]; /**< Text entered in the input area. */
    char defaultText[256]; /**< Default text of the input area. */
} InputArea;

/**
 * @struct Font
 * @brief Represents a font.
 * 
 * This structure holds information about a font, including its path, size, and SDL TTF font object.
 */
typedef struct Font {
    TTF_Font *font; /**< Pointer to the SDL TTF font object. */
    char path[256]; /**< Path to the font file. */
    int size; /**< Size of the font. */
} Font;

/**
 * @struct Theme
 * @brief Represents a theme.
 * 
 * This structure holds information about a theme, including its font, text color, window skin, and parent frame.
 */
typedef struct Theme {
    Font font; /**< Font used in the theme. */
    SDL_Color textColor; /**< Text color used in the theme. */
    TextureMap windowSkin; /**< Texture map for the window skin. */
    struct Frame *parent; /**< Pointer to the parent frame. */
} Theme;

/**
 * @enum FrameFlags
 * @brief Flags for frame properties.
 * 
 * This enumeration defines various flags that can be used to specify properties of a frame.
 */
typedef enum FrameFlags {
    FRAME_BACKGROUND = 1 << 0, /**< Frame has a background. */
    FRAME_CONTENT = 1 << 1, /**< Frame has content. */
    FRAME_NEEDS_REFRESH = 1 << 2, /**< Frame needs to be refreshed. */
    OVERFLOW_SCROLL = 1 << 3, /**< Frame content can be scrolled. */
    OVERFLOW_VISIBLE = 1 << 4, /**< Frame content overflow is visible. */
    FRAME_VISIBLE = 1 << 5, /**< Frame is visible. */
    FRAME_NEEDS_INIT = 1 << 6, /**< Frame needs to be initialized. */
} FrameFlags;

/**
 * @struct Frame
 * @brief Represents a frame.
 * 
 * This structure holds information about a frame, including its position, size, theme, content, and flags.
 */
typedef struct Frame {
    vec4 overflow; /**< Overflow area of the frame. */
    vec2 relPos; /**< Relative position of the frame. */
    vec2 absPos; /**< Absolute position of the frame. */
    vec2 scale; /**< Scale of the frame. */
    vec2 size; /**< Size of the frame. */
    vec2 contentSize; /**< Size of the content within the frame. */
    vec2 scroll; /**< Scroll position of the frame. */
    vec2 scrollTarget; /**< Target scroll position of the frame. */
    char unit[4]; /**< Unit of measurement for the frame's size and position. */
    char alignment[2]; /**< Alignment of the frame. */
    Theme *theme; /**< Pointer to the theme used by the frame. */
    SDL_Surface *contentSurface; /**< SDL surface for the frame's content. */
    TextureMap contentTexture; /**< Texture map for the frame's content. */
    union {
        Label *label; /**< Pointer to a Label structure if the frame contains a label. */
        Button *button; /**< Pointer to a Button structure if the frame contains a button. */
        InputArea *inputArea; /**< Pointer to an InputArea structure if the frame contains an input area. */
        SelectList *selectList; /**< Pointer to a SelectList structure if the frame contains a select list. */
        ImageFrame *imageFrame; /**< Pointer to an ImageFrame structure if the frame contains an image frame. */
    };
    FrameFlags flags; /**< Flags specifying properties of the frame. */
} Frame;

/**
 * @struct RawColor
 * @brief Represents a raw color.
 * 
 * This structure holds the red, green, blue, and alpha components of a color.
 */
typedef struct RawColor {
    u8 r; /**< Red component of the color. */
    u8 g; /**< Green component of the color. */
    u8 b; /**< Blue component of the color. */
    u8 a; /**< Alpha (transparency) component of the color. */
} RawColor;

/** @} */
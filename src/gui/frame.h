
typedef enum ButtonState {
    BUTTON_STATE_NORMAL,
    BUTTON_STATE_HOVERED,
    BUTTON_STATE_PRESSED,
} ButtonState;

typedef struct ImageFrame {
    char path[256];
} ImageFrame;

typedef struct RadioButton {
    u16 *checked;
    u16 id;
} RadioButton;

typedef struct Button {
    bool *checked;
    union {
        RadioButton *radiobutton;
    };
    ButtonState state;
} Button;

typedef struct SelectList {
    ButtonState state;
    char (*options)[256];
    u16 *selected;
    u16 count;
} SelectList;

typedef struct Label {
    char text[256];
} Label;

typedef struct InputArea {
    ButtonState state;
    char text[256];
    char defaultText[256];
} InputArea;

typedef struct Font {
    TTF_Font *font;
    char path[256];
    int size;
} Font;

typedef struct Theme {
    Font font;
    SDL_Color textColor;
    TextureMap windowSkin;
    struct Frame *parent;
} Theme;

typedef enum FrameFlags {
    FRAME_BACKGROUND = 1 << 0,
    FRAME_CONTENT = 1 << 1,
    FRAME_NEEDS_REFRESH = 1 << 2,
    OVERFLOW_SCROLL = 1 << 3,
    OVERFLOW_VISIBLE = 1 << 4,
    FRAME_VISIBLE = 1 << 5,
} FrameFlags;

typedef struct Frame {
    vec4 overflow;
    vec2 relPos, absPos, scale, size, contentSize, scroll, scrollTarget;
    char unit[4];
    char alignment[2];
    Theme *theme;
    SDL_Surface *contentSurface;
    TextureMap contentTexture;
    union {
        Label * label;
        Button * button;
        InputArea * inputArea;
        SelectList * selectList;
        ImageFrame * imageFrame;
    };
    FrameFlags flags;
} Frame;

typedef struct RawColor {
    u8 r, g, b, a;
} RawColor;
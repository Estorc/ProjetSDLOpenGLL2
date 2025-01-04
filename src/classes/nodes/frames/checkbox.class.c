#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include "gui/frame.h"
#include "io/input.h"

class CheckBox : public Button {
    __containerType__ Node *
    public:

    void constructor() {
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        METHOD(this, init_frame);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load(FILE *file) {
        IGNORE(file);
        METHOD_TYPE(this, __type__, constructor);
        METHOD(this, init_button);
        Frame *frame = (Frame *) this->object;
        frame->scale[0] = 48.0f;
        frame->scale[1] = 48.0f;
        frame->unit[2] = 'p';
        frame->unit[3] = 'p';
        frame->alignment[0] = 'c';
        frame->alignment[1] = 'c';
    }

    void save(FILE *file, Node *editor) {
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void is_checkbox(bool *result) {
        *result = true;
    }

    void free() {
        SUPER(free);
    }
 
    
}
#line 1 "src/__processed__/classes_nodes_frames_frame.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../storage/node.h"
#include "../io/shader.h"
#include "../render/render.h"
#include "../window.h"
#include "../gui/frame.h"
#include "../memory.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_FRAME;
#line 14 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        this->type = __type__; 
        SUPER(initialize_node);
        call_method_0(METHOD(init_frame,this));
}

    

#line 27 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_handle_dimension_unit(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * src = va_arg(args, float *);float * dest = va_arg(args, float *);int  vertical = va_arg(args, int );double  size = va_arg(args, double );int  unit = va_arg(args, int );double  containerWidth = va_arg(args, double );double  containerHeight = va_arg(args, double );(void)this;
        *dest = *src;
        *dest = (vertical && size) ? -*dest : *dest; // Inverser l'axe vertical car OpenGL est en coordonnées inversées
        float axisSize = (vertical) ? containerHeight : containerWidth;
        if (size) *dest *= 2;
        switch (unit) {
        case 'p':
            *dest /= (vertical) ? containerHeight : containerWidth;
            break;
        case 'w':
            *dest /= 1.0f/(((1.0f/axisSize)*containerWidth)/100.0f); // Doit prendre en compte la taille de l'écran
            break;
        case 'h':
            *dest /= 1.0f/(((1.0f/axisSize)*containerHeight)/100.0f); // Doit prendre en compte la taille de l'écran
            break;
        case '%':
            *dest /= 100.0f;
            break;
        }
}

#line 48 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_init_frame(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        frame->alignment[0] = 'l';
        frame->alignment[1] = 't';
        frame->flags = 0;
        frame->flags |= FRAME_VISIBLE;
        frame->flags |= FRAME_NEEDS_REFRESH;
        frame->flags |= FRAME_NEEDS_INIT;
        frame->contentTexture = 0;
        frame->contentSurface = NULL;
        frame->theme = NULL;
        glm_vec2_zero(frame->contentSize);
        glm_vec2_zero(frame->scroll);
        glm_vec2_zero(frame->scrollTarget);
}

#line 64 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        this->type = __type__;
        call_method_0(METHOD(constructor,this));
        Frame *frame = (Frame *) this->object;
        if (file) {
            char scroll;
            char texturePath[256];
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c", 
                &frame->relPos[0],&frame->unit[0], 
                &frame->relPos[1],&frame->unit[1], 
                &frame->scale[0],&frame->unit[2], 
                &frame->scale[1],&frame->unit[3], 
                &frame->alignment[0], &frame->alignment[1],
                &scroll);
            char c = fgetc(file);
            if (c == ',') {
                frame->theme = malloc(sizeof(Theme));
                frame->theme->parent = frame;
                fscanf(file, "[%[^,],%[^,],%d,#%2hhx%2hhx%2hhx%2hhx])",	 
                    texturePath,
                    frame->theme->font.path,
                    &frame->theme->font.size,
                    &frame->theme->textColor.r, &frame->theme->textColor.g, &frame->theme->textColor.b, &frame->theme->textColor.a);
                frame->theme->font.font = TTF_OpenFont(frame->theme->font.path, frame->theme->font.size);
                frame->theme->windowSkin = load_texture_from_path(texturePath, GL_SRGB_ALPHA, true);
            }

            frame->flags |= FRAME_BACKGROUND;
            if (scroll == 's') {
                frame->flags |= OVERFLOW_SCROLL;
            } else if (scroll == 'v') {
                frame->flags |= OVERFLOW_VISIBLE;
            }
        }
}

#line 100 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_refresh(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;

        int window_width, window_height;
        get_resolution(&window_width, &window_height);
        vec2 containerSize = {window_width, window_height};
        Frame *frame = (Frame *) this->object;
        vec2 scroll = {0.0f, 0.0f};

        glm_vec4_copy((vec4) {0.0f, 0.0f, containerSize[0], containerSize[1]}, frame->overflow);

        for (int i = 0; i < this->length; i++) {
            bool is_child_gui = false;
            call_method_0(METHOD(is_gui_element,(this->children[i]),&is_child_gui));
            if (is_child_gui) {
                Frame * childFrame = (Frame *) this->children[i]->object;
                childFrame->flags |= FRAME_NEEDS_REFRESH;
            }
        }

        bool is_parent_gui = false;
        if (this->parent) {
            call_method_0(METHOD(is_gui_element,(this->parent),&is_parent_gui));
        }
        if (is_parent_gui) {
            Frame * parentFrame = (Frame *) this->parent->object;
            if (!frame->theme) frame->theme = parentFrame->theme;
            containerSize[0] = parentFrame->size[0];
            containerSize[1] = parentFrame->size[1];
            if (!(parentFrame->flags & OVERFLOW_VISIBLE)) {
                glm_vec2_copy(parentFrame->scroll, scroll);
                glm_vec4_copy(parentFrame->overflow, frame->overflow);

                vec4 overflow;
                overflow[0] = parentFrame->absPos[0];
                overflow[1] = parentFrame->absPos[1];
                overflow[2] = parentFrame->size[0] + parentFrame->absPos[0];
                overflow[3] = parentFrame->size[1] + parentFrame->absPos[1];

                if (frame->overflow[0] < overflow[0]) frame->overflow[0] = overflow[0];
                if (frame->overflow[1] < overflow[1]) frame->overflow[1] = overflow[1];
                if (frame->overflow[2] > overflow[2]) frame->overflow[2] = overflow[2];
                if (frame->overflow[3] > overflow[3]) frame->overflow[3] = overflow[3];
            }
        }

        call_method_0(METHOD(handle_dimension_unit,this,&frame->scale[0], &this->scale[0], false, 0.0f, frame->unit[2], (double) containerSize[0], (double) containerSize[1]));
        call_method_0(METHOD(handle_dimension_unit,this,&frame->scale[1], &this->scale[1], true, 0.0f, frame->unit[3], (double) containerSize[0], (double) containerSize[1]));
        call_method_0(METHOD(handle_dimension_unit,this,&frame->relPos[0], &this->pos[0], false, this->scale[0], frame->unit[0], (double) containerSize[0], (double) containerSize[1]));
        call_method_0(METHOD(handle_dimension_unit,this,&frame->relPos[1], &this->pos[1], true, this->scale[1], frame->unit[1], (double) containerSize[0], (double) containerSize[1]));

        glm_vec2_add(this->pos, scroll, this->pos);

        frame->absPos[0] = this->pos[0];
        frame->absPos[1] = -this->pos[1]; // Inverser l'axe vertical car OpenGL est en coordonnées inversées

        switch (frame->alignment[0]) {
            case 'l':
                this->pos[0] += -1.0f + this->scale[0];
                break;
            case 'c':
                frame->absPos[0] += 1.0f-this->scale[0];
                break;
            case 'r':
                frame->absPos[0] += 2.0f-this->scale[0]*2.0f;
                this->pos[0] += 1.0f - this->scale[0];
                break;
        }

        switch (frame->alignment[1]) {
            case 'b':
                frame->absPos[1] += 2.0f-this->scale[1]*2.0f;
                this->pos[1] += -1 + this->scale[1];
                break;
            case 'c':
                frame->absPos[1] += 1.0f-this->scale[1];
                break;
            case 't':
                this->pos[1] += 1 - this->scale[1];
                break;
        }

        frame->absPos[0] *= containerSize[0]/2.0f;
        frame->absPos[1] *= containerSize[1]/2.0f;


        frame->size[0] = this->scale[0]*containerSize[0];
        frame->size[1] = this->scale[1]*containerSize[1];
        if (is_parent_gui) {
            Frame * parentFrame = (Frame *) this->parent->object;
            frame->absPos[0] += parentFrame->absPos[0];
            frame->absPos[1] += parentFrame->absPos[1];
            if (parentFrame->contentSize[0] < this->scale[0] + this->pos[0] - scroll[0]) parentFrame->contentSize[0] = this->scale[0] + this->pos[0] - scroll[0];
            if (parentFrame->contentSize[1] < this->scale[1] - this->pos[1] + scroll[1]) parentFrame->contentSize[1] = this->scale[1] - this->pos[1] + scroll[1];
        }
        
        frame->flags &= ~FRAME_NEEDS_REFRESH;
}

#line 198 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_refreshContent(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        glBindTexture(GL_TEXTURE_2D, frame->contentTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, frame->contentSurface->w, frame->contentSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->contentSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
}

#line 206 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        if (frame->flags & OVERFLOW_SCROLL) {
            frame->scrollTarget[1] -= input.mouse.scroll_y/8.0f;
            if (frame->scrollTarget[1] < 0.0f) frame->scrollTarget[1] = 0.0f;
            if (frame->scrollTarget[1] > frame->contentSize[1] - 1.0f) frame->scrollTarget[1] = frame->contentSize[1] - 1.0f;
            frame->scroll[1] += (frame->scrollTarget[1] - frame->scroll[1]) * 0.5f;
            if (fabs(frame->scrollTarget[1] - frame->scroll[1]) <= 0.01f) frame->scroll[1] = frame->scrollTarget[1]; // Prevents never-ending scrolling
            if (input.mouse.scroll_y || frame->scrollTarget[1] != frame->scroll[1]) frame->flags |= FRAME_NEEDS_REFRESH;
        }
        if (window.resized) frame->flags |= FRAME_NEEDS_REFRESH;
}


#line 220 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_draw_frame(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        VAO vao;
        call_method_0(METHOD(get_vao,this,&vao));
        glBindVertexArray(vao);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glBindVertexArray(0);
}

    static VBO _vbo = 0;
    static VAO _vao = 0;
#line 235 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);Shader  activeShader = va_arg(args, Shader );WorldShaders * shaders = va_arg(args, WorldShaders *);(void)this;
        IGNORE(modelMatrix);
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_NEEDS_REFRESH || frame->flags & FRAME_NEEDS_INIT) call_method_0(METHOD(refresh,this));
        if (frame->flags & FRAME_VISIBLE && activeShader != shaders->depth && !(frame->flags & FRAME_NEEDS_INIT)) {
            IGNORE(activeShader);
            Frame *frame = (Frame *) this->object;
            use_shader(shaders->gui);

            set_shader_vec2(shaders->gui, "pixelSize", (vec2) {frame->size[0], frame->size[1]});

            bool is_parent_gui = false;
            if (this->parent) {
                call_method_0(METHOD(is_gui_element,(this->parent),&is_parent_gui));
            }

            set_shader_vec2(shaders->gui, "pixelPosition", frame->absPos);
            set_shader_vec4(shaders->gui, "overflow", frame->overflow);
            set_shader_float(shaders->gui, "time", SDL_GetTicks());
            
            set_shader_int(shaders->gui, "background", 0);
            set_shader_int(shaders->gui, "content", 1);

            set_shader_int(shaders->gui, "backgroundEnabled", frame->flags & FRAME_BACKGROUND);
            set_shader_int(shaders->gui, "contentEnabled", frame->flags & FRAME_CONTENT);


            bool is_checkbox = false;
            call_method_0(METHOD(is_checkbox,this,&is_checkbox));
            set_shader_int(shaders->gui, "checked", 0);
            if (is_checkbox) {
                set_shader_int(shaders->gui, "isCheckBox", 1);
            } else set_shader_int(shaders->gui, "isCheckBox", 0);

            if (call_method_2(METHOD(is_radiobutton,this))) {
                RadioButton *radiobutton = frame->button->radiobutton;
                set_shader_int(shaders->gui, "isRadioButton", 1);
                if (radiobutton->checked) set_shader_int(shaders->gui, "checked", !!(*radiobutton->checked & radiobutton->id));
            } else set_shader_int(shaders->gui, "isRadioButton", 0);


            ButtonState state = BUTTON_STATE_NORMAL;
            bool is_element_type = false;

            call_method_0(METHOD(is_button,this,&is_element_type));
            if (is_element_type) {
                if (frame->button->checked) set_shader_int(shaders->gui, "checked", *frame->button->checked);
                state = frame->button->state;
            }

            call_method_0(METHOD(is_input_area,this,&is_element_type));
            if (is_element_type) state = frame->inputArea->state;

            call_method_0(METHOD(is_selectlist,this,&is_element_type));
            if (is_element_type) state = frame->selectList->state;

            set_shader_int(shaders->gui, "pressed", state == BUTTON_STATE_PRESSED);
            set_shader_int(shaders->gui, "hovered", state == BUTTON_STATE_HOVERED);

            if (frame->flags & FRAME_BACKGROUND) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, frame->theme->windowSkin);
            }
            if (frame->flags & FRAME_CONTENT) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, frame->contentTexture);
            }

            set_shader_mat4(shaders->gui, "model", modelMatrix);
            call_method_0(METHOD(draw_frame,this));
        }
        frame->flags &= ~FRAME_NEEDS_INIT;
}

#line 309 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        IGNORE(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);

        fprintf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c%c", 
            frame->relPos[0],frame->unit[0], 
            frame->relPos[1],frame->unit[1], 
            frame->scale[0],frame->unit[2], 
            frame->scale[1],frame->unit[3], 
            frame->alignment[0], frame->alignment[1],
            (frame->flags & OVERFLOW_SCROLL) ? 's' : (frame->flags & OVERFLOW_VISIBLE) ? 'v' : 'n');
        if (frame->theme && frame->theme->parent == frame) {
            fprintf(file, "[");
            TextureMap texture = frame->theme->windowSkin;
            for (int i = 0; i < memoryCaches.texturesCount; i++) {
                if (memoryCaches.textureCache[i].textureMap == texture) {
                    fprintf(file, "%s,", memoryCaches.textureCache[i].textureName);
                    break;
                }
            }
            fprintf(file, "%s,%d,#%2hhx%2hhx%2hhx%2hhx])",	 
                frame->theme->font.path,
                frame->theme->font.size,
                frame->theme->textColor.r, frame->theme->textColor.g, frame->theme->textColor.b, frame->theme->textColor.a);
        } else {
            fprintf(file, ")");
        }
}

#line 339 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_get_vao(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);VAO * vao = va_arg(args, VAO *);(void)this;
        if (!_vao) {
            float quadVertices[] = {
                // positions        // texture Coords
                1.0f,  -1.0f,       0.0f,   1.0f,
                1.0f,   1.0f,       0.0f,   0.0f,
                -1.0f,   1.0f,       1.0f,   0.0f,

                1.0f,  -1.0f,       0.0f,   1.0f,
                -1.0f,   1.0f,       1.0f,   0.0f,
                -1.0f,  -1.0f,       1.0f,   1.0f
            };
            // setup plane VAO
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }
        *vao = _vao;
}


#line 366 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_gui_element(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = true;
}

#line 370 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_button(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = false;
}

#line 374 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_input_area(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = false;
}

#line 378 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_selectlist(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = false;
}

#line 382 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_checkbox(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * result = va_arg(args, bool *);(void)this;
        *result = false;
}

bool call_method_2(void (*func)(void *, va_list), ...) {
    va_list args;
    va_start(args, func);
    bool value;
    func(&value, args);
    va_end(args);
    return value;
}
#line 386 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_is_radiobutton(void * __retValueVP__, va_list args) {bool * __retValueP__ = (bool *) __retValueVP__;Node * this = va_arg(args, Node *);(void)this;
        *__retValueP__ = false;return;
}

#line 390 "src/classes/nodes/frames/frame.class.c"
void __class_method_frame_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        Frame *frame = (Frame *) this->object;
        if (frame->flags & FRAME_CONTENT) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            if (frame->contentTexture) glDeleteTextures(1, &frame->contentTexture);
            if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        }
        if (frame->theme && frame->theme->parent == frame) {
            TTF_CloseFont(frame->theme->font.font);
            free(frame->theme);
        }
        SUPER(free);
}
 
    

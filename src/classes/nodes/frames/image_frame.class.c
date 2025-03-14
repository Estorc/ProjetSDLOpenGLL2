/**
 * @file
 * @brief This file contains the implementation of the ImageFrame class.
 *
 * This file contains the implementation of the ImageFrame class, which is used
 * to handle and manipulate image frames within the application. The ImageFrame
 * class provides functionalities to load, display, and manage image data in
 * the context of the SDL and OpenGL framework.
 *
 * It is used for rendering image frames in the graphical user interface and
 * performing operations related to image processing and display.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "storage/node.h"
#include "io/shader.h"
#include "render/render.h"
#include "window.h"
#include "gui/frame.h"

class ImageFrame : public Frame {
    __containerType__ Node *
    public:

    void constructor(double x, int xu, double y, int yu, double w, int wu, double h, int hu, int ha, int va, char *imagePath) {
        this->type = __type__; 

        Frame *frame;
        frame = malloc(sizeof(Frame));
        POINTER_CHECK(frame);

        this->object = frame;
        SUPER(initialize_node);
        this::init_frame();
        frame->imageFrame = malloc(sizeof(ImageFrame));
        POINTER_CHECK(frame->imageFrame);
        frame->flags &= ~FRAME_BACKGROUND;

        frame->relPos[0] = x;
        frame->relPos[1] = y;
        frame->scale[0] = w;
        frame->scale[1] = h;
        frame->unit[0] = xu;
        frame->unit[1] = yu;
        frame->unit[2] = wu;
        frame->unit[3] = hu;
        frame->alignment[0] = ha;
        frame->alignment[1] = va;

        if (imagePath && *imagePath) {
            frame->contentTexture = load_texture_from_path(imagePath, GL_SRGB_ALPHA, false);
            GLint width, height;
            glBindTexture(GL_TEXTURE_2D, frame->contentTexture);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
            glBindTexture(GL_TEXTURE_2D, 0);
            frame->contentSize[0] = width;
            frame->contentSize[1] = height;
            frame->flags |= FRAME_CONTENT;
        }
    }

    

    void load(FILE *file) {
        float relPos[2], scale[2];
        char unit[4], alignment[2];
        char imagePath[256] = "";
        if (file) {
            fscanf(file, "(%g%c,%g%c,%g%c,%g%c,%c%c, %[^)])", 
                &relPos[0],&unit[0], 
                &relPos[1],&unit[1], 
                &scale[0],&unit[2], 
                &scale[1],&unit[3], 
                &alignment[0], &alignment[1],
                imagePath);
        }
        this::constructor(relPos[0], unit[0], relPos[1], unit[1], scale[0], unit[2], scale[1], unit[3], alignment[0], alignment[1], imagePath);
    }


    void set_image(char *path) {
        Frame *frame = (Frame *) this->object;
        strcpy(frame->imageFrame->path, path);
        frame->contentTexture = load_texture_from_path(path, GL_SRGB_ALPHA, false);
        frame->flags |= FRAME_CONTENT;
    }


    void render(mat4 *modelMatrix, Shader activeShader, WorldShaders *shaders) {
        SUPER(render, modelMatrix, activeShader, shaders);
    }

    void save(FILE *file, Node *editor) {
        Frame *frame = (Frame *) this->object;
        UNUSED(editor);
        fprintf(file, "%s", classManager.class_names[this->type]);
        fprintf(file, "(%s,%c%c)", 
        frame->label->text, frame->alignment[0], frame->alignment[1]);
    }
 
    void free() {
        Frame *frame = (Frame *) this->object;
        ImageFrame *imageFrame = (ImageFrame *) frame->imageFrame;
        free(imageFrame);
        frame->flags &= ~FRAME_CONTENT; // Ensure the glTexture is not deleted
        if (frame->contentSurface) SDL_FreeSurface(frame->contentSurface);
        SUPER(free);
    }
    
}
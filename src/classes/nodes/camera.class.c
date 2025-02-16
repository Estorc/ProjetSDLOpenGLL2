/**
 * @file
 * @brief This file contains the implementation of the Camera class.
 *
 * This source file provides the functionality for managing and manipulating
 * camera objects within the application. It includes functions for setting
 * camera parameters, updating the camera's position and orientation, and
 * handling camera-related transformations.
 *
 * The Camera class is essential for rendering scenes from different viewpoints
 * and is used extensively in the graphics rendering pipeline.
 *
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Camera : public Node {
    __containerType__ Node *
    public:

    void constructor(struct Camera *camera) {
        this->object = camera;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void update(vec3 *pos, vec3 *rot, vec3 *scale) {
        Camera *camera = (Camera *) this->object;

        this::update_global_position(pos, rot, scale);
        glm_vec3_negate_to(*pos, camera->pos);
        glm_vec3_negate_to(*rot, camera->rot);
    }

    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        void *data[] = {
            "camera", "Active : "
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    void load(FILE *file, Camera **c, Script *scripts, Node *editor) {
        UNUSED(scripts);
        Camera *cam;
        cam = malloc(sizeof(Camera));
        POINTER_CHECK(cam);
        init_camera(cam);
        if (file) {
            int active_camera;
            fscanf(file,"(%d)", &active_camera);
            if (active_camera) {
                if (c) *c = cam;
                else if (editor) editor->attribute[5].node = this;
            }
        }
        this->type = __type__;
        this::constructor(cam);
    }

    void save(FILE *file, Node *editor) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->attribute[5].node == this));
        else fprintf(file, "(%d)", 0);
    }




    
}

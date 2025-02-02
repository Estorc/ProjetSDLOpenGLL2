#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_CAMERA;
#line 10 "src/classes/nodes/camera.class.c"
void __class_method_camera_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct Camera * camera = va_arg(args, struct Camera *);(void)this;
        this->object = camera;
        this->type = __type__;
        SUPER(initialize_node);
}

#line 16 "src/classes/nodes/camera.class.c"
void __class_method_camera_update(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);vec3 * pos = va_arg(args, vec3 *);vec3 * rot = va_arg(args, vec3 *);vec3 * scale = va_arg(args, vec3 *);(void)this;
        Camera *camera = (Camera *) this->object;

        this::update_global_position(pos, rot, scale);
        glm_vec3_negate_to(*pos, camera->pos);
        glm_vec3_negate_to(*rot, camera->rot);
}

#line 24 "src/classes/nodes/camera.class.c"
void __class_method_camera_get_settings_data(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);void ***  ptr = va_arg(args, void *** );int *  length = va_arg(args, int * );(void)this;
        SUPER(get_settings_data, ptr, length);
        void *data[] = {
            "camera", "Active : "
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
}

#line 34 "src/classes/nodes/camera.class.c"
void __class_method_camera_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Camera ** c = va_arg(args, Camera **);Script * scripts = va_arg(args, Script *);Node * editor = va_arg(args, Node *);(void)this;
        IGNORE(scripts);
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

#line 52 "src/classes/nodes/camera.class.c"
void __class_method_camera_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);Node * editor = va_arg(args, Node *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
        if (editor) fprintf(file, "(%d)", !!(editor->attribute[5].node == this));
        else fprintf(file, "(%d)", 0);
}




    

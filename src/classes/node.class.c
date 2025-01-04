#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Node {
    __containerType__ Node *
    public:

    void constructor() {
        this->object = NULL;
        this->type = __type__;
        METHOD(this, initialize_node);
    }

    void initialize_node() {
        this->flags = NODE_DEFAULT_FLAGS;
        this->length = 0;
        this->children = NULL;
        this->behavior = NULL;
        this->params = NULL;
        this->params_count = 0;
        this->shader = 0;
        glm_vec3_zero(this->pos);
        glm_vec3_zero(this->rot);
        glm_vec3_one(this->scale);
    }

    static Shader glowShader = 0;
    void get_glow_shader(Shader *shader) {
        if (!glowShader) {
            glowShader = create_shader("shaders/glow.vs", "shaders/glow.fs");
        }
        *shader = glowShader;
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load() {
        METHOD_TYPE(this, __type__, constructor);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    void render() {
        //
    }

    void update() {
        //
    }

    void free() {
        for (int i = 0; i < this->length; i++) {
            METHOD(this->children[i], free);
        }
        free(this->object);
        free(this->params);
        free(this->children);
        free(this);
    }

    void is_cshape(bool *cshape) {
        *cshape = false;
    }

    void is_body(bool *body) {
        *body = false;
    }

    void is_area(bool *area) {
        *area = false;
    }

    void is_gui_element(bool *result) {
        *result = false;
    } 

    int test_function(int a, int b) {
        printf("a: %d, b: %d\n", a, b);
        return a + b;
    }
}
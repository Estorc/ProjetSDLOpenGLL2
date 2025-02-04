#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class CShape : public Node {
    __containerType__ Node *
    public:

    void is_cshape(bool *cshape) {
        (*cshape) = true;
    }

    void get_model(Model **model) {
        *model = NULL;
    }
    
    static Shader collisionShader;
    void get_shader(Shader *shader) {
        if (!collisionShader) {
            collisionShader = create_shader("shaders/collision_debug.vs", "shaders/collision_debug.fs");
        }
        *shader = collisionShader;
    }


    void render(mat4 *modelMatrix, Shader activeShader) {
        Model *model;
        this::get_model(&model);
        Shader shader;
        this::get_shader(&shader);
        use_shader(shader);
        if (model) render_model(modelMatrix, shader, model);
    }

}

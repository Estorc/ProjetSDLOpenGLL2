#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BOXCSHAPE;
#line 10 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);struct BoxCollisionShape * boxCollisionShape = va_arg(args, struct BoxCollisionShape *);(void)this;
        this->object = boxCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_get_priority(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);int * priority = va_arg(args, int *);(void)this;
        *priority = 0;
}

#line 22 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        this->type = __type__;
        this::constructor(boxCollisionShape);
}

#line 30 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}








    static VBO _vbo = 0;
    static VAO _vao = 0;
    static Shader collisionShader = 0;
#line 44 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_get_vao(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);VAO * vao = va_arg(args, VAO *);(void)this;
        if (!_vao) {
            collisionShader = create_shader("shaders/collision_debug.vs", "shaders/collision_debug.fs");
            float boxVertices[] = {
                // Positions          // Normals           // TexCoords  // Tangents          // Bitangents
                // Front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                // Back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   0.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
                // Left face
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,   1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                // Right face
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                // Top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                // Bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   0.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,   1.0f,  0.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            };


            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
        }
        *vao = _vao;
}





#line 117 "src/classes/nodes/cshapes/box_cshape.class.c"
void __class_method_boxcshape_render(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);mat4 * modelMatrix = va_arg(args, mat4 *);(void)this;
        VAO vao;
        this::get_vao(&vao);
        use_shader(collisionShader);

        int modelLoc = glGetUniformLocation(collisionShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        
        glBindVertexArray(vao);
        glDepthFunc(GL_ALWAYS);     // Set depth function to always pass
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);       // Restore default depth function
        glBindVertexArray(0);
}

    

/**
 * @file textured_mesh.class.c
 * @author Loup Picault
 * @date 2023-10-25
 * @brief This file contains the implementation of the TexturedMesh class.
 *
 * The TexturedMesh class is responsible for handling 3D mesh objects with textures.
 * It includes functionalities for loading, rendering, and managing textured meshes
 * within an OpenGL context. This class is a part of the larger project aimed at 
 * creating a 3D graphics application using SDL and OpenGL.
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"

class TexturedMesh : public Node {
    __containerType__ Node *
    public:

    void constructor(struct TexturedMesh *texturedMesh) {
        this->object = texturedMesh;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_settings_data(void *** ptr, int * length) {
        SUPER(get_settings_data, ptr, length);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        void *data[] = {
            "texture", "Texture : ", &texture,
        };
        *ptr = realloc(*ptr, (*length)*sizeof(void *) + sizeof(data));
        memcpy(*ptr + (*length), data, sizeof(data));
        *length += sizeof(data)/sizeof(void *);
    }

    void load(FILE *file) {
        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        char path[100];
        if (file) {
            fscanf(file,"(%100[^)])", path);
        } else {
            path[0] = 0;
        }
        create_textured_plane(texturedMesh, path);
        this->type = __type__;
        this::constructor(texturedMesh);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < memoryCaches.texturesCount; i++) {
            if (memoryCaches.textureCache[i].textureMap == texture) {
                fprintf(file, "(%s)", memoryCaches.textureCache[i].textureName);
                break;
            }
        }
    }


    void render(mat4 *modelMatrix, Shader activeShader) {
        TexturedMesh *texturedMesh = (TexturedMesh*) this->object;
        vec3 defaultColor = {0.5f, 0.5f, 0.5f};
        set_shader_vec3(activeShader, "material.ambient", defaultColor);
        set_shader_vec3(activeShader, "material.specular", defaultColor);
        set_shader_vec3(activeShader, "material.diffuse", defaultColor);
        set_shader_float(activeShader, "material.parallax", 0.5f);

        set_shader_float(activeShader, "material.metallic", 0.0f);
        set_shader_float(activeShader, "material.roughness", 0.5f);

        set_shader_int(activeShader, "normalMapActive", 0);
        set_shader_int(activeShader, "metallicMapActive", 0);
        set_shader_int(activeShader, "roughnessMapActive", 0);

        set_shader_int(activeShader, "diffuseMapActive", 1);

        set_shader_mat4(activeShader, "model", modelMatrix);
        // render Cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturedMesh->texture);
        glBindVertexArray(texturedMesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
        glBindVertexArray(0);
    }

    
}

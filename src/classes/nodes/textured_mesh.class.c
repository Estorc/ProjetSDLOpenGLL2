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

/**
 * @ingroup Classes Classes
 * @{
 */

class TexturedMesh : public Node {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the textured mesh class.
     *
     * This function initializes a textured mesh object using the provided file path.
     *
     * @param path The file path to the texture to be used for the mesh.
     */
    void constructor(const char *path) {
        this->type = __type__;

        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        create_textured_plane(texturedMesh, path);

        this->object = texturedMesh;
        SUPER(initialize_node);
    }

    /**
     * @brief Retrieves settings data.
     *
     * This function retrieves settings data and stores it in the provided pointer.
     *
     * @param[out] ptr A pointer to a pointer to a pointer where the settings data will be stored.
     * @param[out] length A pointer to an integer where the length of the settings data will be stored.
     */
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

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the provided file pointer and loads it into the appropriate structures.
     *
     * @param file A pointer to the FILE object that represents the file to be read.
     */
    void load(FILE *file) {
        char path[100];
        if (file) {
            fscanf(file,"(%100[^)])", path);
        } else {
            path[0] = 0;
        }
        this::constructor(path);
    }

    /**
     * @brief Saves the current state of the textured mesh to a file.
     *
     * This function writes the necessary data of the textured mesh to the provided file.
     *
     * @param file A pointer to the FILE object where the textured mesh data will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < Game.memoryCaches->texturesCount; i++) {
            if (Game.memoryCaches->textureCache[i].textureMap == texture) {
                fprintf(file, "(%s)", Game.memoryCaches->textureCache[i].textureName);
                break;
            }
        }
    }

    /**
     * @brief Renders the textured mesh using the provided model matrix and active shader.
     * 
     * This function takes a model matrix and an active shader as input parameters and
     * renders the textured mesh accordingly. The model matrix is used to transform the
     * mesh, and the shader is used to apply the necessary rendering effects.
     * 
     * @param modelMatrix Model matrix used for transforming the mesh.
     * @param activeShader The shader program used for rendering the mesh.
     */
    void render(mat4 modelMatrix, Shader activeShader) {
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


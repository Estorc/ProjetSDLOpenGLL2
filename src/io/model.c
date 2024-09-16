#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../math/math_util.h"
#include "model.h"
#include "../render/color.h"
#include "../render/camera.h"



/**
 * Creates a textured plane mesh using the specified texture file.
 *
 * @param texturedMesh {TexturedMesh*} A pointer to a TexturedMesh structure that 
 *                                      will hold the created mesh's VAO, texture, 
 *                                      and length.
 * @param texture {char*} The file path of the texture image to be applied to the plane.
 *
 * This function generates a textured plane by creating a Vertex Array Object (VAO)
 * and binding it with a Vertex Buffer Object (VBO) containing the vertex data for 
 * a rectangular plane. The plane is positioned in the 3D space with a specified 
 * texture applied to it.
 *
 * The plane is defined by a series of vertices that include position, normal, and 
 * texture coordinate data:
 * - The vertices are defined to form two triangles (6 vertices total) making up 
 *   the rectangular plane.
 *
 * If the texture is successfully loaded, an OpenGL texture is generated, and its 
 * parameters are set, including wrapping and filtering modes. If the texture fails 
 * to load, an error message is printed.
 *
 * The function initializes the following attributes in the TexturedMesh structure:
 * - VAO: The generated Vertex Array Object for the mesh.
 * - texture: The generated texture ID.
 * - length: The number of triangles in the mesh (6 vertices = 2 triangles).
 *
 * Example Usage:
 * TexturedMesh myPlane;
 * create_textured_plane(&myPlane, "path/to/texture.png");
 * // Use myPlane for rendering...
 *
 * Return:
 * This function does not return a value but modifies the TexturedMesh structure 
 * pointed to by `texturedMesh`.
 */

void create_textured_plane(TexturedMesh *texturedMesh, char *texture) {

    VBO meshVBO;
    VAO meshVAO;
    TextureMap meshTex = load_texture_from_path(texture);
    float vertices[] = {
        25.0f, 0.0f,  25.0f,  0.0f, -1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, 0.0f,  25.0f,  0.0f, -1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, 0.0f, -25.0f,  0.0f, -1.0f, 0.0f,   0.0f, 25.0f,

        25.0f, 0.0f,  25.0f,  0.0f, -1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, 0.0f, -25.0f,  0.0f, -1.0f, 0.0f,   0.0f, 25.0f,
        25.0f, 0.0f, -25.0f,  0.0f, -1.0f, 0.0f,  25.0f, 25.0f    
    };
    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &meshVBO);
    // fill buffer
    glBindVertexArray(meshVAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    /*glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(11 * sizeof(float)));*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texturedMesh->VAO = meshVAO;
    texturedMesh->texture = meshTex;
    texturedMesh->length = 6;
}


/**
 * Creates a fullscreen quad (screen plane) mesh for rendering.
 *
 * @param mesh {Mesh*} A pointer to a Mesh structure that will hold the created 
 *                     mesh's VAO and length.
 *
 * This function generates a fullscreen quad mesh suitable for rendering images 
 * or effects that cover the entire screen. The quad is defined by six vertices 
 * forming two triangles.
 *
 * The vertex data includes positions in normalized device coordinates (NDC) and 
 * corresponding texture coordinates, allowing for the mapping of textures across 
 * the full screen.
 *
 * The function initializes the following attributes in the Mesh structure:
 * - VAO: The generated Vertex Array Object for the mesh.
 * - length: The number of vertices in the mesh (6 vertices total).
 *
 * Example Usage:
 * Mesh screenPlane;
 * create_screen_plane(&screenPlane);
 * // Use screenPlane for rendering...
 *
 * Return:
 * This function does not return a value but modifies the Mesh structure pointed 
 * to by `mesh`.
 */


void create_screen_plane(Mesh *mesh) {
    VBO meshVBO;
    VAO meshVAO;

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    // setup plane VAO
    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &meshVBO);
    glBindVertexArray(meshVAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    mesh->VAO = meshVAO;
    mesh->length = 6;
}
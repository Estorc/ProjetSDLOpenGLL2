#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "memory.h"
#include "render/render.h"

class Skybox : public Node {
    __containerType__ Node *
    public:

    void constructor(struct TexturedMesh *texturedMesh) {
        this->object = texturedMesh;
        this->type = __type__;
        SUPER(initialize_node);
    }

    
    /**
     * @brief Loads a cubemap texture from 6 individual texture faces.
     * 
     * This function takes an array of file paths to 6 texture images, each representing
     * one face of a cubemap (right, left, top, bottom, front, back), and loads them into
     * a single cubemap texture.
     * 
     * @param faces An array of 6 strings, each containing the file path to a texture image.
     *              The order of the faces should be: right, left, top, bottom, front, back.
     * 
     * @return TextureMap The loaded cubemap texture.
     */

    TextureMap load_cubemap(char (*faces)[100]) {

        for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
            if (!strcmp(memoryCaches.cubeMapCache[i].textureName[0], faces[0]) &&
                !strcmp(memoryCaches.cubeMapCache[i].textureName[1], faces[1]) &&
                !strcmp(memoryCaches.cubeMapCache[i].textureName[2], faces[2]) &&
                !strcmp(memoryCaches.cubeMapCache[i].textureName[3], faces[3]) &&
                !strcmp(memoryCaches.cubeMapCache[i].textureName[4], faces[4]) &&
                !strcmp(memoryCaches.cubeMapCache[i].textureName[5], faces[5])) {
                #ifdef DEBUG
                    printf("Cube Map loaded from cache!\n");
                #endif
                return memoryCaches.cubeMapCache[i].cubeMap;
            }
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int success = 1;

        for (unsigned int i = 0; i < 6; i++) {
            SDL_Surface* textureSurface = IMG_Load(faces[i]);

            if (textureSurface) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                            0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels
                );
                SDL_FreeSurface(textureSurface);
            } else {
                printf("Cubemap tex failed to load at path: %s\n", faces[i]);
                SDL_FreeSurface(textureSurface);
                success = 0;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        if (success) {
            memoryCaches.cubeMapCache = realloc(memoryCaches.cubeMapCache, sizeof (CubeMapCache) * (++memoryCaches.cubeMapCount));
            memoryCaches.cubeMapCache[memoryCaches.cubeMapCount-1].cubeMap = textureID;
            for (unsigned int i = 0; i < 6; i++) {
                strcpy(memoryCaches.cubeMapCache[memoryCaches.cubeMapCount-1].textureName[i], faces[i]);
            }
        }
        return textureID;
    }  


    /**
     * @brief Creates a skybox with the given textures.
     *
     * This function initializes a skybox using the provided textures and assigns it to the given textured mesh.
     *
     * @param texturedMesh A pointer to the TextureMap structure where the skybox will be created.
     * @param skyboxTexture A 2D array containing the file paths of the six textures for the skybox. 
     *                      The textures should be ordered as follows: right, left, top, bottom, front, back.
     * @note Each file path in the skyboxTexture array should not exceed 200 characters.
     */

    void create_skybox(TexturedMesh *texturedMesh, char (*skyboxTexture)[100]) {

        VBO skyboxVBO;
        VAO skyboxVAO;

        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        // fill buffer
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        // link vertex attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        texturedMesh->VAO = skyboxVAO;
        texturedMesh->length = 36;
        texturedMesh->texture = this::(TextureMap)load_cubemap(skyboxTexture);

    }

    void load(FILE *file) {
        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        char path[6][100];
        if (file) {
            fscanf(file,"(%100[^,],%100[^,],%100[^,],%100[^,],%100[^,],%100[^)])", 
            path[0],
            path[1],
            path[2],
            path[3],
            path[4],
            path[5]
            );
        } else {
            path[0][0] = path[1][0] = path[2][0] = path[3][0] = path[4][0] = path[5][0] = 0;
        }
        this::create_skybox(texturedMesh, &path);
        this->type = __type__;
        this::constructor(texturedMesh);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
        TextureMap texture = ((TexturedMesh*) this->object)->texture;
        for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
            if (memoryCaches.cubeMapCache[i].cubeMap == texture) {
                fprintf(file, "(%s,%s,%s,%s,%s,%s)",
                    memoryCaches.cubeMapCache[i].textureName[0],
                    memoryCaches.cubeMapCache[i].textureName[1],
                    memoryCaches.cubeMapCache[i].textureName[2],
                    memoryCaches.cubeMapCache[i].textureName[3],
                    memoryCaches.cubeMapCache[i].textureName[4],
                    memoryCaches.cubeMapCache[i].textureName[5]
                );
                break;
            }
        }
    }


    void render(mat4 *modelMatrix, Shader activeShader, WorldShaders *shaders) {
        IGNORE(activeShader);
        
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

        Shader shader = shaders->skybox;
        use_shader(shader);
        int modelLoc = glGetUniformLocation(shader, "model");
        TexturedMesh *texturedMesh = (TexturedMesh *)this->object;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        // render Cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texturedMesh->texture);
        set_shader_int(shader, "skybox", 0);
        glBindVertexArray(texturedMesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, texturedMesh->length);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS); // set depth function back to default
    }

}
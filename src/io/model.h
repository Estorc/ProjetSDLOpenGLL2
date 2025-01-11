#ifndef MODEL_H
#define MODEL_H


/*
 * To ensure MTL files are loaded correctly, the following rules must be followed:
 * - The MTL file must be in the same directory as the OBJ file.
 * - The MTL file must have the same name as the OBJ file.
 * - The MTL file must have the extension ".mtl".
 * - The MTL file must not contain this regex string: "(-s \d* \d* \d* *)*(-bm \d*\.*\d* )*""
 */

/**
 * @file model.h
 * @brief Defines structures and functions for handling 3D models and their properties.
 */

/**
 * @defgroup ModelHandling Model Handling
 * @brief Group of functions and structures for handling 3D models.
 * @{
 */


/**
 * @def MAX_VERTEX_PER_FACE
 * @brief Maximum number of vertices per face.
 */
#define MAX_VERTEX_PER_FACE 10

/**
 * @typedef VBO
 * @brief Alias for unsigned int representing a Vertex Buffer Object.
 */
typedef unsigned int VBO;

/**
 * @typedef VAO
 * @brief Alias for unsigned int representing a Vertex Array Object.
 */
typedef unsigned int VAO;

/**
 * @typedef VertexBuffer
 * @brief Alias for VBO.
 */
typedef VBO VertexBuffer;

/**
 * @typedef VertexArray
 * @brief Alias for VAO.
 */
typedef VAO VertexArray;

/**
 * @typedef Vertex
 * @brief Array of 15 floats representing a vertex.
 */
typedef float Vertex[15];

/**
 * @typedef Normal
 * @brief Alias for vec3 representing a normal vector.
 */
typedef vec3 Normal;

/**
 * @typedef TextureVertex
 * @brief Alias for vec2 representing a texture vertex.
 */
typedef vec2 TextureVertex;

/**
 * @typedef TextureMap
 * @brief Alias for u32 representing a texture map.
 */
typedef u32 TextureMap;


/**
 * @enum MaterialProperties
 * @brief Enum representing different material properties.
 * 
 * This enum is used to identify various material properties such as diffuse, specular, ambient, emission, etc.
 * Each property can be associated with a texture map or a flat color.
 */
typedef enum {
    DIFFUSE_MATERIAL_PROPERTY,              /**< Diffuse material property (Kd / map_Kd) */
    SPECULAR_MATERIAL_PROPERTY,             /**< Specular material property (Ks / map_Ks) */
    AMBIENT_MATERIAL_PROPERTY,              /**< Ambient material property (Ka) */
    EMISSION_MATERIAL_PROPERTY,             /**< Emission material property (Ke / map_Ke) */
    PARALLAX_MATERIAL_PROPERTY,             /**< Parallax material property (Px / map_Px) */
    ROUGHNESS_MATERIAL_PROPERTY,            /**< Roughness material property (Pr / map_Pr) */
    SHEEN_MATERIAL_PROPERTY,                /**< Sheen material property (Ps / map_Ps) */
    METALLIC_MATERIAL_PROPERTY,             /**< Metallic material property (Pm / map_Pm) */
    CLEARCOAT_THICKNESS_MATERIAL_PROPERTY,  /**< Clearcoat thickness material property (Pc) */
    CLEARCOAT_ROUGHNESS_MATERIAL_PROPERTY,  /**< Clearcoat roughness material property (Pcr) */
    NORMAL_MATERIAL_PROPERTY,               /**< Normal material property (bump / map_Bump) */
    REFLECTION_MATERIAL_PROPERTY,           /**< Reflection material property (refl) */
    ANISOTROPY_MATERIAL_PROPERTY,           /**< Anisotropy material property (aniso) */
    ANISOTROPY_ROTATION_MATERIAL_PROPERTY,  /**< Anisotropy rotation material property (anisor) */
    MATERIAL_PROPERTY_COUNT                 /**< Total count of material properties */
} MaterialProperties;

/**
 * @struct Material
 * @brief Structure representing a material with various properties.
 * 
 * This structure holds the name, flat colors, texture maps, and other properties of a material.
 */
typedef struct Material {
    char name[128];                         /**< Name of the material */
    vec3 flatColors[MATERIAL_PROPERTY_COUNT]; /**< Array of flat colors for each material property */
    TextureMap textureMaps[MATERIAL_PROPERTY_COUNT]; /**< Array of texture maps for each material property */
    f32 specularExp;                        /**< Specular exponent (Ns) */
    f32 opacity;                            /**< Opacity (d) */
    f32 opticalDensity;                     /**< Optical density (Ni) */
    u8 illumFlag;                           /**< Illumination flag (illum) */
} Material;

/**
 * @struct Face
 * @brief Structure representing a face of a 3D model.
 * 
 * This structure holds the indices of vertices, normals, and texture vertices that make up a face.
 */
typedef struct Face {
    int vertex[MAX_VERTEX_PER_FACE];        /**< Indices of vertices */
    int normals[MAX_VERTEX_PER_FACE];       /**< Indices of normals */
    int textureVertex[MAX_VERTEX_PER_FACE]; /**< Indices of texture vertices */
    u8 length;                              /**< Number of vertices in the face */
} Face;

/**
 * @struct ModelObjectData
 * @brief Structure representing the data of a 3D model object.
 * 
 * This structure holds the vertices, normals, texture vertices, faces, and other properties of a model object.
 */
typedef struct ModelObjectData {
    Vertex *vertex;                         /**< Array of vertices */
    Normal *normals;                        /**< Array of normals */
    TextureVertex *textureVertex;           /**< Array of texture vertices */
    Vertex (*facesVertex)[3];               /**< Array of vertex indices for faces */
    Face *faces;                            /**< Array of faces */
    u32 *textures;                          /**< Array of texture IDs */
    u8 smoothShading;                       /**< Smooth shading flag */
    u32 length;                             /**< Length of the object data */
    VAO VAO;                                /**< Vertex Array Object */
    Material **materials;                   /**< Array of pointers to materials */
    u32 *materialsLength;                   /**< Array of material lengths */
    u8 materialsCount;                      /**< Number of materials */
    GLuint *displayLists;                   /**< Array of display lists */
} ModelObjectData;

/**
 * @struct ModelData
 * @brief Structure representing the data of a 3D model.
 * 
 * This structure holds the materials and objects that make up a 3D model.
 */
typedef struct ModelData {
    Material *materials;                    /**< Array of materials */
    u8 materialsCount;                      /**< Number of materials */
    ModelObjectData *objects;               /**< Array of model objects */
    u8 length;                              /**< Number of objects */
} ModelData;

/**
 * @enum ModelFlags
 * @brief Enum representing flags for a model.
 * 
 * This enum is used to set various flags for a model, such as glow effect.
 */
typedef enum ModelFlags {
    MODEL_FLAG_NONE         = 0,            /**< No flags */
    MODEL_FLAG_GLOW         = 1 << 0,       /**< Glow effect flag */
    MODEL_FLAG_UNUSED1      = 1 << 1,       /**< Unused flag 1 */
    MODEL_FLAG_UNUSED2      = 1 << 2,       /**< Unused flag 2 */
    MODEL_FLAG_UNUSED3      = 1 << 3,       /**< Unused flag 3 */
    MODEL_FLAG_UNUSED4      = 1 << 4,       /**< Unused flag 4 */
    MODEL_FLAG_UNUSED5      = 1 << 5,       /**< Unused flag 5 */
    MODEL_FLAG_UNUSED6      = 1 << 6,       /**< Unused flag 6 */
    MODEL_FLAG_UNUSED7      = 1 << 7        /**< Unused flag 7 */
} ModelFlags;

/**
 * @struct Model
 * @brief Structure representing a 3D model.
 * 
 * This structure holds the data and flags of a 3D model.
 */
typedef struct Model {
    struct ModelData *data;                 /**< Pointer to the model data */
    u8 flags;                               /**< Flags for the model */
} Model;

/**
 * @struct TexturedMesh
 * @brief Structure representing a textured mesh.
 * 
 * This structure holds the Vertex Array Object (VAO) and texture map of a textured mesh.
 */
typedef struct TexturedMesh {
    VAO VAO;                                /**< Vertex Array Object */
    TextureMap texture;                     /**< Texture map */
    u8 length;                              /**< Length of the textured mesh */
} TexturedMesh;

/**
 * @struct Mesh
 * @brief Structure representing a mesh.
 * 
 * This structure holds the Vertex Array Object (VAO) of a mesh.
 */
typedef struct Mesh {
    VAO VAO;                                /**< Vertex Array Object */
    u8 length;                              /**< Length of the mesh */
} Mesh;

/**
 * @brief Creates a textured plane.
 * 
 * This function initializes a textured plane with the given texture.
 * 
 * @param texturedMesh Pointer to the TexturedMesh structure to be initialized.
 * @param texture Path to the texture file.
 */
void create_textured_plane(TexturedMesh *texturedMesh, char *texture);

/**
 * @brief Creates a screen plane.
 * 
 * This function initializes a screen plane.
 * 
 * @param mesh Pointer to the Mesh structure to be initialized.
 */
void create_screen_plane(Mesh *mesh);

/** @} */ // end of ModelHandling


#include "obj_loader.h"
#include "mtl_loader.h"

#endif
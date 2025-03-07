#ifndef MODEL_H
#define MODEL_H

#include "shader.h"


/*
 * To ensure MTL files are loaded correctly, the following rules must be followed:
 * - The MTL file must be in the same directory as the OBJ file.
 * - The MTL file must have the same name as the OBJ file.
 * - The MTL file must have the extension ".mtl".
 * - The MTL file must not contain this regex string: "(-s \d* \d* \d* *)*(-bm \d*\.*\d* )*"
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

typedef enum VertexAttribute {
    VERTEX_ATTRIBUTE_POSITION_X,
    VERTEX_ATTRIBUTE_POSITION_Y,
    VERTEX_ATTRIBUTE_POSITION_Z,
    VERTEX_ATTRIBUTE_NORMAL_X,
    VERTEX_ATTRIBUTE_NORMAL_Y,
    VERTEX_ATTRIBUTE_NORMAL_Z,
    VERTEX_ATTRIBUTE_TEXTURE_U,
    VERTEX_ATTRIBUTE_TEXTURE_V,
    VERTEX_ATTRIBUTE_TANGENT_X,
    VERTEX_ATTRIBUTE_TANGENT_Y,
    VERTEX_ATTRIBUTE_TANGENT_Z,
    VERTEX_ATTRIBUTE_BITANGENT_X,
    VERTEX_ATTRIBUTE_BITANGENT_Y,
    VERTEX_ATTRIBUTE_BITANGENT_Z,
    VERTEX_ATTRIBUTE_BONE_ID_1,
    VERTEX_ATTRIBUTE_BONE_ID_2,
    VERTEX_ATTRIBUTE_BONE_ID_3,
    VERTEX_ATTRIBUTE_BONE_ID_4,
    VERTEX_ATTRIBUTE_BONE_WEIGHT_1,
    VERTEX_ATTRIBUTE_BONE_WEIGHT_2,
    VERTEX_ATTRIBUTE_BONE_WEIGHT_3,
    VERTEX_ATTRIBUTE_BONE_WEIGHT_4,
    VERTEX_ATTRIBUTE_COUNT
} VertexAttribute;

/**
 * @typedef Vertex
 * @brief Array of VERTEX_ATTRIBUTE_COUNT floats representing a vertex.
 */
typedef float Vertex[VERTEX_ATTRIBUTE_COUNT];

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
 * @struct ModelObjectData
 * @brief Structure representing the data of a 3D model object.
 * 
 * This structure holds the vertices, normals, texture vertices, faces, and other properties of a model object.
 */
typedef struct ModelObjectData ModelObjectData;
struct ModelObjectData {
    Vertex *facesVertex;                    /**< Array of vertex indices for faces */
    u32 length;                             /**< Length of the object data */
    Material **materials;                   /**< Array of pointers to materials */
    u32 *materialsLength;                   /**< Array of material lengths */ //The numbers of faces before switching material
    u32 materialsCount;                     /**< Number of materials */
    VAO VAO;                                /**< Vertex Array Object */
    GLuint *displayLists;                   /**< Array of display lists */
};


/**
 * @struct ModelKeyframe
 * @brief Structure representing the data of a 3D model keyframe.
 * 
 * This structure holds the transformation matrix and time of a keyframe.
 */

typedef struct ModelKeyframe {
    union {
        vec3 value;                         /**< Value of the keyframe */
        versor r_value;                     /**< Rotation of the keyframe */
    };
    f32 time;                               /**< Time of the keyframe */
} ModelKeyframe;

/**
 * @struct Bone
 * @brief Structure representing a bone in a 3D model.
 * 
 * This structure holds the name, parent, and children of a bone in a 3D model.
 */

typedef struct Bone {
    char name[128];                         /**< Name of the bone */
    mat4 offsetMatrix;                      /**< Offset matrix */
    mat4 transform;                         /**< Transformation matrix */
    ModelKeyframe *translationKeyframes;    /**< Array of translation keyframes */
    u32 translationKeyframesCount;          /**< Number of translation keyframes */
    ModelKeyframe *rotationKeyframes;       /**< Array of rotation keyframes */
    u32 rotationKeyframesCount;             /**< Number of rotation keyframes */
    ModelKeyframe *scaleKeyframes;          /**< Array of scale keyframes */
    u32 scaleKeyframesCount;                /**< Number of scale keyframes */
    u32 index;                              /**< Index of the bone */
} Bone;

/**
 * @struct ModelAnimation
 * @brief Structure representing the data of a 3D model animation.
 * 
 * This structure holds the keyframes and other properties of a model animation.
 */

typedef struct ModelAnimation ModelAnimation;
struct ModelAnimation {
    char name[128];                         /**< Name of the animation */
    u32 length;                             /**< Length of the animation */
    Bone *targets;                          /**< Array of targets */
    u32 targetsCount;                       /**< Number of targets */
};

/**
 * @struct ModelData
 * @brief Structure representing the data of a 3D model.
 * 
 * This structure holds the materials and objects that make up a 3D model.
 */
typedef struct ModelData ModelData;
struct ModelData {
    Material *materials;                    /**< Array of materials */ //UNUSED
    u8 materialsCount;                      /**< Number of materials */ //UNUSED
    ModelObjectData *objects;               /**< Array of model objects */
    u32 length;                              /**< Number of objects */
    ModelAnimation *animations;             /**< Array of model animations */
    u8 animationsCount;                     /**< Number of animations */
};

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
void create_textured_plane(TexturedMesh *texturedMesh, const char *texture);

/**
 * @brief Creates a screen plane.
 * 
 * This function initializes a screen plane.
 * 
 * @param mesh Pointer to the Mesh structure to be initialized.
 */
void create_screen_plane(Mesh *mesh);


void render_model(mat4 *modelMatrix, Shader activeShader, Model *model);

/** @} */ // end of ModelHandling


#include "model_loaders/model_loader.h"

#endif
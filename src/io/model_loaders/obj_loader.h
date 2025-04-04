#pragma once

/**
 * @defgroup ObjLoader OBJ Loader
 * @brief Functions for loading and managing 3D models from OBJ files.
 * @{
 */

/**
 * @def MAX_VERTEX_PER_FACE
 * @brief Maximum number of vertices per face.
 */
#define MAX_VERTEX_PER_FACE 10

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
 * @struct LoadingModelBuffer
 * @brief Structure to hold the data of a 3D model being loaded.
 *
 * This structure contains arrays of vertices, normals, texture vertices, and faces
 * that represent a 3D model. It is used as a buffer during the loading process of
 * an OBJ file or similar 3D model formats.
 *
 * @var LoadingModelBuffer::vertex
 * Array of vertices that define the geometric points of the model.
 *
 * @var LoadingModelBuffer::normals
 * Array of normals that define the direction perpendicular to the surface of the model's faces.
 *
 * @var LoadingModelBuffer::textureVertex
 * Array of texture vertices that map the 2D texture coordinates to the 3D model.
 *
 * @var LoadingModelBuffer::faces
 * Array of faces that define the polygons of the model, typically triangles or quads.
 */
typedef struct LoadingModelBuffer {
    Vertex *vertex;                         /**< Array of vertices */
    Normal *normals;                        /**< Array of normals */
    TextureVertex *textureVertex;           /**< Array of texture vertices */
    Face *faces;                            /**< Array of faces */
} LoadingModelBuffer;

/**
 * @brief Allocates memory for an ModelObjectData's vertex, face, normal, and texture data.
 *
 * @param obj {ModelObjectData*} A pointer to the ModelObjectData structure to be initialized.
 * @param vim {u32} The initial number of vertices to allocate memory for.
 * @param fim {u32} The initial number of faces to allocate memory for.
 * @param vnim {u32} The initial number of normals to allocate memory for.
 * @param vtim {u32} The initial number of texture coordinates to allocate memory for.
 *
 * This function allocates the necessary memory for various data fields in 
 * the specified ModelObjectData, including vertices, faces, normals, and texture 
 * coordinates. It initializes the materials and materialsLength to NULL 
 * and sets the materialsCount to zero.
 *
 * Important Notes:
 * - The function assumes that `obj` is a valid pointer to an ModelObjectData 
 *   structure.
 * - If any memory allocation fails, a pointer check function (POINTER_CHECK) 
 *   should handle the error, potentially freeing previously allocated memory 
 *   and terminating the program or logging the error.
 *
 * Example Usage:
 * malloc_obj(&object, initialVertexCount, initialFaceCount, 
 *             initialNormalCount, initialTextureVertexCount);
 *
 * @return Returns 0 on successful memory allocation.
 */
int malloc_obj(ModelObjectData *obj, LoadingModelBuffer *buffer, u32 vim, u32 fim, u32 vnim, u32 vtim);

/**
 * @brief Cleans up and reallocates the memory for the vertex data and faces of a 3D 
 * object, and generates the Vertex Array Object (VAO) for rendering.
 *
 * @param obj {ModelObjectData*} A pointer to the ModelObjectData structure to be cleaned up and 
 *                      reallocated.
 * @param vi {u32} The current number of vertices loaded for the object.
 * @param fi {u32} The current number of faces loaded for the object.
 * @param vni {u32} The current number of normals loaded for the object.
 * @param vti {u32} The current number of texture coordinates loaded for the object.
 *
 * This function frees the previously allocated memory for normals, texture 
 * vertices, and vertex data associated with the given ModelObjectData structure. It 
 * then reallocates the facesVertex array to fit the actual number of faces 
 * and generates the corresponding Vertex Array Object (VAO) for rendering.
 *
 * Important Notes:
 * - This function assumes that the ModelObjectData structure has been allocated 
 *   and initialized properly before calling.
 * - After calling this function, the obj->facesVertex will point to a 
 *   reallocated array suitable for the number of faces currently stored.
 * - Any previously allocated memory for normals, texture vertices, and 
 *   vertex data will be released, preventing memory leaks.
 * - It is the caller's responsibility to ensure that obj is valid and that 
 *   vi, fi, vni, and vti are correctly provided to reflect the current 
 *   state of the object.
 *
 * Example Usage:
 * close_realloc_obj(&object, vi, fi, vni, vti);
 */
void close_realloc_obj(ModelObjectData *obj, LoadingModelBuffer *buffer, u32 vi, u32 fi, u32 vni, u32 vti);

/**
 * @brief Loads a 3D model from an OBJ file, including its associated materials, 
 * vertices, normals, and texture coordinates.
 *
 * @param path {char*} The directory path where the OBJ file is located.
 * @param filename {char*} The name of the OBJ file to be loaded.
 * @param model {ModelData*} A pointer to a Model structure that will be populated 
 *                       with the loaded data.
 *
 * @return {int} Returns 0 on success, or -1 if an error occurs during loading.
 *
 * This function reads the specified OBJ file and extracts geometric data 
 * such as vertices, texture coordinates, normals, and faces. It also handles 
 * the associated materials through a corresponding MTL file, updating the 
 * Model structure accordingly. The model is dynamically allocated and resized 
 * as necessary to accommodate the loaded data.
 *
 * Important Notes:
 * - The function relies on external utility functions such as `load_mtl`, 
 *   `find_material`, `malloc_obj`, and `close_realloc_obj` for loading 
 *   materials and managing dynamic memory.
 * - The input OBJ file must follow the standard OBJ format; otherwise, 
 *   parsing may fail, leading to potential memory leaks if not handled correctly.
 * - Ensure that sufficient memory is available for the model structure and 
 *   associated objects, as this function dynamically allocates memory for 
 *   various arrays within the Model structure.
 *
 * Example Usage:
 * ModelData model;
 * if (load_obj_model("assets/models/", "example.obj", &model) == 0) {
 *     // Successfully loaded the model
 * } else {
 *     // Handle error
 * }
 */
int load_obj_model(const char *path, ModelData *modelPtr);


/** @} */
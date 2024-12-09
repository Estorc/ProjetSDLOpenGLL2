#ifndef MODEL_H
#define MODEL_H

#define MAX_VERTEX_PER_FACE 10


typedef unsigned int VBO;
typedef unsigned int VAO;
typedef VBO VertexBuffer;
typedef VAO VertexArray;

typedef float Vertex[15];
typedef Vec3f Normal;
typedef Vec2f TextureVertex;
typedef u32 TextureMap;

/*typedef struct {
    Vec3f vec;
} Vertex;*/

typedef enum {
    DIFFUSE_MATERIAL_PROPERTY,
    NORMAL_MATERIAL_PROPERTY,
    PARALLAX_MATERIAL_PROPERTY,
    ROUGHNESS_MATERIAL_PROPERTY,
    METALLIC_MATERIAL_PROPERTY,
    AMBIENT_MATERIAL_PROPERTY,
    SPECULAR_MATERIAL_PROPERTY,
    EMISSION_MATERIAL_PROPERTY,
    HEIGHT_MATERIAL_PROPERTY,
    REFLECTION_MATERIAL_PROPERTY,
    REFRACTION_MATERIAL_PROPERTY,
    MATERIAL_PROPERTY_COUNT
} MaterialProperties;

typedef struct Material {
    char name[20]; // On pourrait imaginer une somme des lettres en exposant ou une conversion en base 64 / MD5 pour obtenir un identifiant unique plus simple à retrouver.
    Vec3f flatColors[MATERIAL_PROPERTY_COUNT];
    TextureMap textureMaps[MATERIAL_PROPERTY_COUNT];
    f32 specularExp;
    f32 opacity;
    f32 opticalDensity;
    u8 illumFlag;
} Material;

typedef struct Face {
    int vertex[MAX_VERTEX_PER_FACE];
    int normals[MAX_VERTEX_PER_FACE];
    int textureVertex[MAX_VERTEX_PER_FACE];
    u8 length;
} Face;

typedef struct ObjectMesh {
    Vertex *vertex;
    Normal *normals;
    TextureVertex *textureVertex;
    Vertex (*facesVertex)[3];
    Face *faces;
    u32 *textures;
    u8 smoothShading;
    u32 length;
    VAO VAO;
    Material **materials;
    u32 *materialsLength;
    u8 materialsCount;
} ObjectMesh;

typedef struct Model {
    Material *materials;
    u8 materialsCount;
    ObjectMesh *objects;
    u8 length;
} Model;

typedef struct TexturedMesh {
    VAO VAO;
    TextureMap texture;
    u8 length;
} TexturedMesh;

typedef struct Mesh {
    VAO VAO;
    u8 length;
} Mesh;

/*typedef struct {
    Material *materials;
    Vertex *vertex;
    Normal *normals;
    TextureVertex *textureVertex;
    Face *faces;
    u32 *textures;
    Vec3f pos;
    Vec3f rot;
    Vec3f scale;
    u8 smoothShading;
} Model;*/

int load_obj_model(char *path, Model **modelPtr);
int load_mtl(char *path, char *filename, Material **materials);
int find_material(Material *materials, int materialsCount, char *materialName);

void create_textured_plane(TexturedMesh *texturedMesh, char *texture);
void create_screen_plane(Mesh *mesh);
//Vec3f get_face_center(Face *face);
//void sort_faces_by_distance(ObjectMesh *obj, Vec3f *vec);

#endif
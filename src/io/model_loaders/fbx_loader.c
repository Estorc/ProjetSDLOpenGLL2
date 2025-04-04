#include <ufbx/ufbx.h>
#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <io/shader.h>
#include <memory.h>

#define MAX_WEIGHTS 4

typedef struct skin_vertex {
	int bone_index[MAX_WEIGHTS];
	float bone_weight[MAX_WEIGHTS];
} skin_vertex;

static inline void copy_ufbxmat4_to_cglmmat4(ufbx_real a[12], float (*dest)[4]) {
    for (int i = 0; i < 16; i++) {
        dest[i/4][i%4] = a[i];
    }
}

static inline void copy_ufbxvec4_to_cglmvec4(ufbx_quat a, float * dest) {
    dest[0] = a.x;
    dest[1] = a.y;
    dest[2] = a.z;
    dest[3] = a.w;
}

static inline void copy_ufbxvec3_to_cglmvec3(ufbx_vec3 a, float * dest) {
    dest[0] = a.x;
    dest[1] = a.y;
    dest[2] = a.z;
}

static inline void copy_ufbxvec2_to_cglmvec2(ufbx_vec2 a, float * dest) {
    dest[0] = a.x;
    dest[1] = a.y;
}


static inline void load_material_texture(const char * folder_path, const ufbx_texture *texture, Material *dest_material, const MaterialProperties property, const GLenum format)
{
    char * texture_path;
    char * raw_relative_path;
    raw_relative_path = convert_path(texture->relative_filename.data);
    texture_path = concat_path(folder_path, raw_relative_path);
    PRINT_INFO("Loading texture: %s\n", texture_path);
    dest_material->textureMaps[property] = load_texture_from_path(texture_path, format, true);
    free(texture_path);
    free(raw_relative_path);
}

static void convert_skin_deformer(ufbx_skin_deformer *skin, skin_vertex *mesh_skin_vertices)
{

    PRINT_INFO("Skin vertices: %zu\n", skin->vertices.count);
    for (size_t i = 0; i < skin->vertices.count; i++) {
        ufbx_skin_vertex skin_vertex = skin->vertices.data[i];

        for (size_t j = 0; j < MAX_WEIGHTS; j++) {
            if (j < skin_vertex.num_weights) {
                ufbx_skin_weight weight = skin->weights.data[skin_vertex.weight_begin+j];
                mesh_skin_vertices[i].bone_index[j] = weight.cluster_index;
                mesh_skin_vertices[i].bone_weight[j] = weight.weight;
            } else {
                mesh_skin_vertices[i].bone_index[j] = -1;
                mesh_skin_vertices[i].bone_weight[j] = 0.0f;
            }
        }

        // Normalize weights if there are less than MAX_WEIGHTS
        float total_weight = 0.0f;
        for (size_t j = 0; j < MAX_WEIGHTS; j++) {
            total_weight += mesh_skin_vertices[i].bone_index[j];
        }
        if (total_weight > 0.0f) {
            for (size_t j = 0; j < MAX_WEIGHTS; j++) {
                mesh_skin_vertices[i].bone_weight[j] /= total_weight;
            }
        }
    }
}

static void convert_material_part(const char * folder_path, ufbx_material *material, Material **dest_material_ptr)
{
    *dest_material_ptr = malloc(sizeof(Material));
    Material * dest_material = *dest_material_ptr;

    for (int i = 0; i < MATERIAL_PROPERTY_COUNT; i++) {
        glm_vec3_copy((vec3) {0.5f, 0.5f, 0.5f}, dest_material->flatColors[i]);
        dest_material->textureMaps[i] = 0;
    }

    /* FBX Materials */
    copy_ufbxvec3_to_cglmvec3(material->fbx.ambient_color.value_vec3, dest_material->flatColors[AMBIENT_MATERIAL_PROPERTY]);
    copy_ufbxvec3_to_cglmvec3(material->fbx.diffuse_color.value_vec3, dest_material->flatColors[DIFFUSE_MATERIAL_PROPERTY]);
    copy_ufbxvec3_to_cglmvec3(material->fbx.specular_color.value_vec3, dest_material->flatColors[SPECULAR_MATERIAL_PROPERTY]);
    copy_ufbxvec3_to_cglmvec3(material->fbx.emission_color.value_vec3, dest_material->flatColors[EMISSION_MATERIAL_PROPERTY]);
    if (material->fbx.diffuse_color.texture)
        load_material_texture(folder_path, material->fbx.diffuse_color.texture, dest_material, DIFFUSE_MATERIAL_PROPERTY, GL_SRGB8_ALPHA8);
    if (material->fbx.specular_color.texture)
        load_material_texture(folder_path, material->fbx.specular_color.texture, dest_material, SPECULAR_MATERIAL_PROPERTY, GL_SRGB8);
    if (material->fbx.emission_color.texture)
        load_material_texture(folder_path, material->fbx.emission_color.texture, dest_material, EMISSION_MATERIAL_PROPERTY, GL_SRGB8);
    if (material->fbx.bump.texture)
        load_material_texture(folder_path, material->fbx.bump.texture, dest_material, NORMAL_MATERIAL_PROPERTY, GL_R8);
    if (material->fbx.displacement.texture)
        load_material_texture(folder_path, material->fbx.displacement.texture, dest_material, PARALLAX_MATERIAL_PROPERTY, GL_R8);

    /* PBR Materials */

    if (material->pbr.metalness.texture)
        load_material_texture(folder_path, material->pbr.metalness.texture, dest_material, METALLIC_MATERIAL_PROPERTY, GL_R8);
    if (material->pbr.sheen_color.texture)
        load_material_texture(folder_path, material->pbr.sheen_color.texture, dest_material, SHEEN_MATERIAL_PROPERTY, GL_R8);
    if (material->pbr.roughness.texture)
        load_material_texture(folder_path, material->pbr.roughness.texture, dest_material, ROUGHNESS_MATERIAL_PROPERTY, GL_R8);
    dest_material->opacity = material->pbr.opacity.value_real;
}


static void convert_mesh_part(ufbx_mesh *mesh, ufbx_mesh_part *part, ModelObjectData *model, skin_vertex *mesh_skin_vertices)
{

    size_t num_triangles = part->num_triangles;
    size_t vertices_shift = model->length;
    size_t num_vertices = vertices_shift;
    PRINT_INFO("Num triangles: %zu\n", num_triangles);
    model->facesVertex = realloc(model->facesVertex, (num_triangles * 3 + vertices_shift) * sizeof(Vertex));

    // Reserve space for the maximum triangle indices.
    size_t num_tri_indices = mesh->max_face_triangles * 3;
    uint32_t *tri_indices = calloc(num_tri_indices, sizeof(uint32_t));

    // Iterate over each face using the specific material.
    for (size_t face_ix = 0; face_ix < part->num_faces; face_ix++) {
        ufbx_face face = mesh->faces.data[part->face_indices.data[face_ix]];

        // Triangulate the face into `tri_indices[]`.
        uint32_t num_tris = ufbx_triangulate_face(tri_indices, num_tri_indices, mesh, face);

        // Iterate over each triangle corner contiguously.
        for (size_t j = 0; j < num_tris; j++) {
            for (size_t i = 0; i < 3; i++) {
                uint32_t index = tri_indices[i+j*3];

                Vertex *v = &model->facesVertex[num_vertices++];

                //PRINT_INFO("%d, %d\n", index, v->indices.count);
                copy_ufbxvec3_to_cglmvec3(ufbx_get_vertex_vec3(&mesh->vertex_position, index),    (*v)+VERTEX_ATTRIBUTE_POSITION_X);
                copy_ufbxvec3_to_cglmvec3(ufbx_get_vertex_vec3(&mesh->vertex_normal, index),      (*v)+VERTEX_ATTRIBUTE_NORMAL_X);
                copy_ufbxvec2_to_cglmvec2(ufbx_get_vertex_vec2(&mesh->vertex_uv, index),          (*v)+VERTEX_ATTRIBUTE_TEXTURE_U);

				memcpy((*v)+VERTEX_ATTRIBUTE_BONE_ID_1, mesh_skin_vertices[mesh->vertex_indices.data[index]].bone_index, sizeof(int)*MAX_WEIGHTS);
                memcpy((*v)+VERTEX_ATTRIBUTE_BONE_WEIGHT_1, mesh_skin_vertices[mesh->vertex_indices.data[index]].bone_weight, sizeof(float)*MAX_WEIGHTS);

            }
            vec2 deltaUV1;
            vec2 deltaUV2;
            glm_vec2_sub(&model->facesVertex[num_vertices-2][VERTEX_ATTRIBUTE_TEXTURE_U], &model->facesVertex[num_vertices-3][VERTEX_ATTRIBUTE_TEXTURE_U], deltaUV1);
            glm_vec2_sub(&model->facesVertex[num_vertices-1][VERTEX_ATTRIBUTE_TEXTURE_U], &model->facesVertex[num_vertices-3][VERTEX_ATTRIBUTE_TEXTURE_U], deltaUV2);
            vec3 edge1;
            vec3 edge2;
            glm_vec3_sub(&model->facesVertex[num_vertices-2][VERTEX_ATTRIBUTE_POSITION_X], &model->facesVertex[num_vertices-3][VERTEX_ATTRIBUTE_POSITION_X],edge1);
            glm_vec3_sub(&model->facesVertex[num_vertices-1][VERTEX_ATTRIBUTE_POSITION_X], &model->facesVertex[num_vertices-3][VERTEX_ATTRIBUTE_POSITION_X],edge2);

            vec3 tangent;
            vec3 bitangent;
            double f = 1.0 / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);

            tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
            tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
            tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
            glm_vec3_normalize(tangent);

            bitangent[0] = f * (deltaUV1[0] * edge2[0] - deltaUV2[0] * edge1[0]);
            bitangent[1] = f * (deltaUV1[0] * edge2[1] - deltaUV2[0] * edge1[1]);
            bitangent[2] = f * (deltaUV1[0] * edge2[2] - deltaUV2[0] * edge1[2]);
            glm_vec3_normalize(bitangent);

            for (int j = 0; j < 3; j++) {
                memcpy(&model->facesVertex[num_vertices-3+j][VERTEX_ATTRIBUTE_TANGENT_X], tangent, sizeof(tangent));
                memcpy(&model->facesVertex[num_vertices-3+j][VERTEX_ATTRIBUTE_BITANGENT_X], bitangent, sizeof(bitangent));
            }
        }
    }

    // Should have written all the vertices.
    free(tri_indices);
    assert(num_vertices == num_triangles * 3 + vertices_shift);

    model->length += num_vertices - vertices_shift;
    model->materialsCount++;
    model->materialsLength = realloc(model->materialsLength, (model->materialsCount) * sizeof(u32));
    model->materialsLength[model->materialsCount-1] = (num_vertices - vertices_shift)/3;
}


void bake_animation(ufbx_scene *scene, ufbx_anim *anim, ModelAnimation *model_anim)
{
    ufbx_baked_anim *bake = ufbx_bake_anim(scene, anim, NULL, NULL);
    assert(bake);
    
    PRINT_INFO("  duration: %f\n", bake->playback_duration);
    model_anim->length = bake->playback_duration;
    model_anim->targetsCount = bake->nodes.count;
    model_anim->targets = malloc(sizeof(Bone) * bake->nodes.count);

    for (size_t i = 0; i < bake->nodes.count; i++) {

        Bone *bone = &model_anim->targets[i];

        ufbx_baked_node *bake_node = &bake->nodes.data[i];
        ufbx_node *scene_node = scene->nodes.data[bake_node->typed_id];

        copy_ufbxmat4_to_cglmmat4(ufbx_transform_to_matrix(&scene_node->local_transform).v, bone->offsetMatrix);

        bone->translationKeyframesCount = bake_node->translation_keys.count;
        bone->rotationKeyframesCount = bake_node->rotation_keys.count;
        bone->scaleKeyframesCount = bake_node->scale_keys.count;

        bone->translationKeyframes = malloc(sizeof(ModelKeyframe) * bake_node->translation_keys.count);
        bone->rotationKeyframes = malloc(sizeof(ModelKeyframe) * bake_node->rotation_keys.count);
        bone->scaleKeyframes = malloc(sizeof(ModelKeyframe) * bake_node->scale_keys.count);

        for (size_t j = 0; j < bake_node->translation_keys.count; j++) {
            bone->translationKeyframes[j].time = bake_node->translation_keys.data[j].time;
            copy_ufbxvec3_to_cglmvec3(bake_node->translation_keys.data[j].value, bone->translationKeyframes[j].value);
        }
        for (size_t j = 0; j < bake_node->rotation_keys.count; j++) {
            bone->rotationKeyframes[j].time = bake_node->rotation_keys.data[j].time;
            copy_ufbxvec4_to_cglmvec4(bake_node->rotation_keys.data[j].value, bone->rotationKeyframes[j].r_value);
        }
        for (size_t j = 0; j < bake_node->scale_keys.count; j++) {
            bone->scaleKeyframes[j].time = bake_node->scale_keys.data[j].time;
            copy_ufbxvec3_to_cglmvec3(bake_node->scale_keys.data[j].value, bone->scaleKeyframes[j].value);
        }

        bone->index = bake_node->typed_id;


        PRINT_INFO("  node %s:\n", scene_node->name.data);
        PRINT_INFO("    translation: %zu keys\n", bake_node->translation_keys.count);
        PRINT_INFO("    rotation: %zu keys\n", bake_node->rotation_keys.count);
        PRINT_INFO("    scale: %zu keys\n", bake_node->scale_keys.count);
    }

    ufbx_free_baked_anim(bake);
}

void bake_animations(ufbx_scene *scene, ModelData *model)
{
    model->animations = NULL;
    // Iterate over every animation stack (aka. clip/take) in the file
    for (size_t i = 0; i < scene->anim_stacks.count; i++) {
        model->animationsCount++;
        model->animations = realloc(model->animations, model->animationsCount * sizeof(ModelAnimation));
        ufbx_anim_stack *stack = scene->anim_stacks.data[i];
        PRINT_INFO("stack %s:\n", stack->name.data);
        strcpy(model->animations[i].name, stack->name.data);
        bake_animation(scene, stack->anim, &model->animations[i]);
    }
}


int load_fbx_armature(const ufbx_node *node, ModelData *model, int index) {
    for (size_t j = 0; j < node->children.count; j++) {
        ufbx_node *child = node->children.data[j];
        for (int i = 0; i < index; i++) printf(" ");
        PRINT_INFO(" -> child: %s\n", child->name.data);
        load_fbx_armature(child, model, index+1);
    }
    return 0;
}


int load_fbx_model(const char *path, ModelData *model) {

    ufbx_load_opts opts = { 0 }; // Optional, pass NULL for defaults
    ufbx_error error; // Optional, pass NULL if you don't care about errors
    char * folder_path = get_folder_path(path);
    ufbx_scene *scene = ufbx_load_file(path, &opts, &error);
    if (!scene) {
        PRINT_ERROR("Failed to load: %s\n", error.description.data);
        exit(1);
    }

    // Use and inspect `scene`, it's just plain data!

    // Let's just list all objects within the scene for example:
    for (size_t i = 0; i < scene->nodes.count; i++) {
        ufbx_node *node = scene->nodes.data[i];
        if (node->is_root || !node->parent->is_root) continue;

        PRINT_INFO("Object: %s\n", node->name.data);
        if (node->mesh) {
            model->objects = malloc(sizeof(ModelObjectData));
            model->length = 1;
            ModelObjectData *object = &model->objects[0];
            object->length = 0;
            object->materials = malloc(sizeof(Material*)*node->mesh->material_parts.count);
            object->materialsCount = 0;
            object->materialsLength = NULL;
            object->facesVertex = NULL;
            skin_vertex *mesh_skin_vertices = malloc(sizeof(skin_vertex)*node->mesh->num_vertices);
            for (int i = 0; i < node->mesh->skin_deformers.count; i++) {
                convert_skin_deformer(node->mesh->skin_deformers.data[i], mesh_skin_vertices);
            }
            for (int i = 0; i < node->mesh->material_parts.count; i++) {
                convert_material_part(folder_path, node->mesh->materials.data[i], &object->materials[i]);
                convert_mesh_part(node->mesh, &node->mesh->material_parts.data[i], object, mesh_skin_vertices);
            }
            for (int i = 0; i < object->length; i++) {
                for (int j = 0; j < 4; j++) {
                    int test;
                    memcpy(&test, &object->facesVertex[i][VERTEX_ATTRIBUTE_BONE_ID_1 + j], sizeof(int));
                    PRINT_INFO("%d ", test);
                }
            }
            object->length /= 3;
            PRINT_INFO("-> mesh with %zu faces\n", node->mesh->faces.count);
        } else if (node->bone) {
            PRINT_INFO("-> bone \"%s\"\n", node->bone->name.data);
        } else {
            PRINT_INFO("-> armature \"%s\"\n", node->name.data);
            load_fbx_armature(node, model, 1);
        }
    }

    bake_animations(scene, model);

    free(folder_path);
    ufbx_free_scene(scene);
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include "stringio.h"
#include "../types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "../math/math_util.h"
#include "model.h"
#include "../render/framebuffer.h"
#include "../node.h"
#include "../render/render.h"
#include "../render/lighting.h"
#include "../window.h"
#include "input.h"
#include "../render/camera.h"
#include "shader.h"
#include "../render/depth_map.h"
#include "scene_loader.h"
#include "../memory.h"
#include "../buffer.h"

void malloc_node(Node *node, int nodeType, FILE *file, Camera **c, Script scripts[SCRIPTS_COUNT], Node *editor) {
    
    switch (nodeType) {
        case NODE_EMPTY:
            {
            create_empty_node(node);
            }
            break;

        case NODE_CAMERA:
            {
            Camera *cam;
            cam = malloc(sizeof(Camera));
            POINTER_CHECK(cam);
            init_camera(cam);
            if (file) {
                int active_camera;
                fscanf(file,"(%d)", &active_camera);
                if (active_camera) {
                    if (c) *c = cam;
                    else if (editor) editor->params[5].node = node;
                }
            }
            create_camera_node(node, cam);
            }
            break;

        case NODE_BOX_CSHAPE:
            {
            BoxCollisionShape *boxCollisionShape;
            boxCollisionShape = malloc(sizeof(BoxCollisionShape));
            POINTER_CHECK(boxCollisionShape);
            create_box_collision_shape_node(node, boxCollisionShape);
            }
            break;

        case NODE_PLANE_CSHAPE:
            {
            PlaneCollisionShape *planeCollisionShape;
            planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
            POINTER_CHECK(planeCollisionShape);
            create_plane_collision_shape_node(node, planeCollisionShape);
            }
            break;

        case NODE_SPHERE_CSHAPE:
            {
            SphereCollisionShape *sphereCollisionShape;
            sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
            POINTER_CHECK(sphereCollisionShape);
            create_sphere_collision_shape_node(node, sphereCollisionShape);
            }
            break;

        case NODE_KINEMATIC_BODY:
            {
            KinematicBody *kinematicBody;
            kinematicBody = malloc(sizeof(KinematicBody));
            kinematicBody->length = 0;
            int children_count = 0;
            POINTER_CHECK(kinematicBody);
            if (file) {
                fscanf(file,"(%f,%f,%f,%d)\n", 
                    &kinematicBody->velocity[0], &kinematicBody->velocity[1], &kinematicBody->velocity[2], 
                    &children_count);
            } else {
                glm_vec3_zero(kinematicBody->velocity);
            }
            create_kinematic_body_node(node, kinematicBody);

            kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
            buffers.collisionBuffer.length += children_count;
            POINTER_CHECK(kinematicBody->collisionsShapes);
            
            for (int i = 0; i < children_count; i++) {
                Node *child = load_node(file, c, scripts, editor);
                kinematicBody->collisionsShapes[kinematicBody->length++] = child;
                child->parent = node;
            }
            }
            break;

        case NODE_RIGID_BODY:
            {
            RigidBody *rigidBody;
            rigidBody = malloc(sizeof(RigidBody));
            rigidBody->length = 0;
            int children_count = 0;
            POINTER_CHECK(rigidBody);
            if (file) {
                fscanf(file,"(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d)\n", 
                    &rigidBody->velocity[0], &rigidBody->velocity[1], &rigidBody->velocity[2], 
                    &rigidBody->angularVelocity[0], &rigidBody->angularVelocity[1], &rigidBody->angularVelocity[2], 
                    &rigidBody->gravity[0], &rigidBody->gravity[1], &rigidBody->gravity[2],
                    &rigidBody->mass,
                    &rigidBody->friction,
                    &rigidBody->centerOfMass[0], &rigidBody->centerOfMass[1], &rigidBody->centerOfMass[2],
                    &children_count);
            } else {
                glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->velocity);
                glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->angularVelocity);
                glm_vec3_copy((vec3) {0.0,-1.0,0.0}, rigidBody->gravity);
                rigidBody->mass = 100.0;
                rigidBody->friction = 0.98;
                glm_vec3_copy((vec3) {0.0,0.0,0.0}, rigidBody->centerOfMass);
            }
            create_rigid_body_node(node, rigidBody);

            rigidBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
            buffers.collisionBuffer.length += children_count;
            POINTER_CHECK(rigidBody->collisionsShapes);
            
            for (int i = 0; i < children_count; i++) {
                Node *child = load_node(file, c, scripts, editor);
                rigidBody->collisionsShapes[rigidBody->length++] = child;
                child->parent = node;
            }
            }
            break;

        case NODE_STATIC_BODY:
            {
            StaticBody *staticBody;
            staticBody = malloc(sizeof(StaticBody));
            staticBody->length = 0;
            int children_count = 0;
            POINTER_CHECK(staticBody);
            if (file)
                fscanf(file,"(%d)\n", &children_count);
            create_static_body_node(node, staticBody);

            staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
            buffers.collisionBuffer.length += children_count;
            POINTER_CHECK(staticBody->collisionsShapes);
            
            for (int i = 0; i < children_count; i++) {
                Node *child = load_node(file, c, scripts, editor);
                staticBody->collisionsShapes[staticBody->length++] = child;
                child->parent = node;
            }
            }
            break;

        case NODE_MESH:
            {
            Mesh *mesh;
            mesh = malloc(sizeof(Mesh));
            POINTER_CHECK(mesh);
            create_screen_plane(mesh);
            create_mesh_node(node, mesh);
            }
            break;

        case NODE_MODEL:
            {
            Model *model;
            if (file) {
                char path[100];
                strcpy(path, RELATIVE_PATH);
                fscanf(file,"(%100[^)])", path);
                load_obj_model(path, &model);
            }
            create_model_node(node, model);
            }
            break;

        case NODE_TEXTURED_MESH:
            {
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
            create_textured_mesh_node(node, texturedMesh);
            }
            break;

        case NODE_SKYBOX:
            {
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
            create_skybox(texturedMesh, path);
            create_skybox_node(node, texturedMesh);
            }
            break;

        case NODE_VIEWPORT:
            {
            Viewport *viewport;
            viewport = malloc(sizeof(Viewport));

            Mesh *mesh;
            mesh = malloc(sizeof(Mesh));
            POINTER_CHECK(mesh);
            create_screen_plane(mesh);
            Node *screenPlaneNode;
            screenPlaneNode = malloc(sizeof(Node));
            create_mesh_node(screenPlaneNode, mesh);
            viewport->screenPlane = screenPlaneNode;
            
            create_viewport_node(node, viewport);
            }
            break;

        case NODE_FRAMEBUFFER:
            {
            FrameBuffer *msaa_framebuffer;
            msaa_framebuffer = malloc(sizeof(FrameBuffer));
            POINTER_CHECK(msaa_framebuffer);
            FrameBuffer *msaa_framebuffer_intermediate;
            msaa_framebuffer_intermediate = malloc(sizeof(FrameBuffer));
            POINTER_CHECK(msaa_framebuffer_intermediate);
            Filter *msaa_framebuffer_filter;
            msaa_framebuffer_filter = malloc(sizeof(Filter));
            POINTER_CHECK(msaa_framebuffer_filter);

            msaa_framebuffer_filter->beginScript = anti_aliasing_begin_script;
            msaa_framebuffer_filter->endScript = anti_aliasing_end_script;
            msaa_framebuffer_filter->flags = FILTER_DEFAULT_FLAGS;
            create_msaa_framebuffer(msaa_framebuffer, msaa_framebuffer_intermediate, &msaa_framebuffer_filter->texture);

            create_filter_node(node, msaa_framebuffer_filter);

            node->children = realloc(node->children, sizeof(Node *) * 2);
            POINTER_CHECK(node->children);

            Node *frameBufferMSAA;
            frameBufferMSAA = malloc(sizeof(Node));
            POINTER_CHECK(frameBufferMSAA);
            add_child(node, create_framebuffer_node(frameBufferMSAA, msaa_framebuffer));
            Node *frameBufferMSAAIntermediate;
            frameBufferMSAAIntermediate = malloc(sizeof(Node));
            POINTER_CHECK(frameBufferMSAAIntermediate);
            add_child(node, create_framebuffer_node(frameBufferMSAAIntermediate, msaa_framebuffer_intermediate));
            }
            break;

        case NODE_POINT_LIGHT:
            {
            PointLight *pointLight;
            pointLight = malloc(sizeof(PointLight));
            POINTER_CHECK(pointLight);

            if (file) {
                fscanf(file,"(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)\n", 
                    &pointLight->ambient[0], &pointLight->ambient[1], &pointLight->ambient[2], 
                    &pointLight->diffuse[0], &pointLight->diffuse[1], &pointLight->diffuse[2], 
                    &pointLight->specular[0], &pointLight->specular[1], &pointLight->specular[2],
                    &pointLight->constant,
                    &pointLight->linear,
                    &pointLight->quadratic
                    );
            } else {
                glm_vec3_zero(pointLight->ambient);
                glm_vec3_copy(GLM_VEC3_ONE, pointLight->diffuse);
                glm_vec3_copy(GLM_VEC3_ONE, pointLight->specular);
                pointLight->constant = 1.0f;
                pointLight->linear = 0.09f;
                pointLight->quadratic = 0.032f;
            }

            buffers.lightingBuffer.length++;
            create_point_light_node(node, pointLight);
            node->flags |= NODE_EDITOR_FLAG;
            }
            break;

        case NODE_DIRECTIONAL_LIGHT:
            {
            DirectionalLight *directionalLight;
            directionalLight = malloc(sizeof(DirectionalLight));
            POINTER_CHECK(directionalLight);

            if (file) {
                fscanf(file,"(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)\n", 
                    &directionalLight->ambient[0], &directionalLight->ambient[1], &directionalLight->ambient[2], 
                    &directionalLight->diffuse[0], &directionalLight->diffuse[1], &directionalLight->diffuse[2], 
                    &directionalLight->specular[0], &directionalLight->specular[1], &directionalLight->specular[2],
                    &directionalLight->constant,
                    &directionalLight->linear,
                    &directionalLight->quadratic
                    );
            } else {
                glm_vec3_zero(directionalLight->ambient);
                glm_vec3_copy(GLM_VEC3_ONE, directionalLight->diffuse);
                glm_vec3_copy(GLM_VEC3_ONE, directionalLight->specular);
                directionalLight->constant = 1.0f;
                directionalLight->linear = 0.09f;
                directionalLight->quadratic = 0.032f;
            }

            buffers.lightingBuffer.length++;
            create_directional_light_node(node, directionalLight);
            node->flags |= NODE_EDITOR_FLAG;
            }
            break;

        case NODE_SPOT_LIGHT:
            {
            SpotLight *spotLight;
            spotLight = malloc(sizeof(SpotLight));
            POINTER_CHECK(spotLight);

            if (file) {
                fscanf(file,"(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)\n", 
                    &spotLight->ambient[0], &spotLight->ambient[1], &spotLight->ambient[2], 
                    &spotLight->diffuse[0], &spotLight->diffuse[1], &spotLight->diffuse[2], 
                    &spotLight->specular[0], &spotLight->specular[1], &spotLight->specular[2],
                    &spotLight->constant,
                    &spotLight->linear,
                    &spotLight->quadratic,
                    &spotLight->cutOff,
                    &spotLight->outerCutOff
                    );
            } else {
                glm_vec3_zero(spotLight->ambient);
                glm_vec3_copy(GLM_VEC3_ONE, spotLight->diffuse);
                glm_vec3_copy(GLM_VEC3_ONE, spotLight->specular);
                spotLight->constant = 1.0f;
                spotLight->linear = 0.09f;
                spotLight->quadratic = 0.032f;
                spotLight->cutOff = 0.01f;
                spotLight->outerCutOff = 50.0f;
            }

            buffers.lightingBuffer.length++;
            create_spot_light_node(node, spotLight);
            node->flags |= NODE_EDITOR_FLAG;
            }
            break;
    }
}


void node_tree_to_file(FILE * file, Node *node, Node *editor) {

    int nodeType = node->type;
    int relativePathLength = strlen(RELATIVE_PATH);
    
    switch (nodeType) {
        case NODE_EMPTY:
            {
            fprintf(file, "empty");
            }
            break;

        case NODE_CAMERA:
            {
            fprintf(file, "camera");
            if (editor) fprintf(file, "(%d)", !!(editor->params[5].node == node));
            else fprintf(file, "(%d)", 0);
            }
            break;

        case NODE_BOX_CSHAPE:
            {
            fprintf(file, "cbox");
            }
            break;

        case NODE_PLANE_CSHAPE:
            {
            fprintf(file, "cplane");
            }
            break;

        case NODE_SPHERE_CSHAPE:
            {
            fprintf(file, "csphere");
            }
            break;

        case NODE_KINEMATIC_BODY:
            {
            KinematicBody *kinematicBody = (KinematicBody*) node->object;
            fprintf(file, "kbody");
            u8 *collisionsLength;
            GET_FROM_BODY_NODE(node, length, collisionsLength);
            fprintf(file, "(%f,%f,%f,%d)",
            kinematicBody->velocity[0], kinematicBody->velocity[1], kinematicBody->velocity[2], 
            *collisionsLength);
            }
            break;

        case NODE_RIGID_BODY:
            {
            RigidBody *rigidBody = (RigidBody*) node->object;
            fprintf(file, "rbody");
            u8 *collisionsLength;
            GET_FROM_BODY_NODE(node, length, collisionsLength);
            fprintf(file, "(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d)",
            rigidBody->velocity[0], rigidBody->velocity[1], rigidBody->velocity[2], 
            rigidBody->angularVelocity[0], rigidBody->angularVelocity[1], rigidBody->angularVelocity[2], 
            rigidBody->gravity[0], rigidBody->gravity[1], rigidBody->gravity[2],
            rigidBody->mass,
            rigidBody->friction,
            rigidBody->centerOfMass[0], rigidBody->centerOfMass[1], rigidBody->centerOfMass[2],
            *collisionsLength);
            }
            break;

        case NODE_STATIC_BODY:
            {
            fprintf(file, "sbody");
            u8 *collisionsLength;
            GET_FROM_BODY_NODE(node, length, collisionsLength);
            fprintf(file, "(%d)", *collisionsLength);
            }
            break;

        case NODE_MESH:
            {
            fprintf(file, "m");
            }
            break;

        case NODE_MODEL:
            {
            Model *model = (Model*) node->object;
            fprintf(file, "mdl");
            for (int i = 0; i < memoryCaches.modelsCount; i++) {
                if (memoryCaches.modelCache[i].model == model) {
                    fprintf(file, "(%s)", memoryCaches.modelCache[i].modelName+relativePathLength);
                    break;
                }
            }
            }
            break;

        case NODE_TEXTURED_MESH:
            {
            TextureMap texture = ((TexturedMesh*) node->object)->texture;
            fprintf(file, "tp");
            for (int i = 0; i < memoryCaches.texturesCount; i++) {
                if (memoryCaches.textureCache[i].textureMap == texture) {
                    fprintf(file, "(%s)", memoryCaches.textureCache[i].textureName+relativePathLength);
                    break;
                }
            }
            }
            break;

        case NODE_SKYBOX:
            {
            TextureMap texture = ((TexturedMesh*) node->object)->texture;
            fprintf(file, "sky");
            for (int i = 0; i < memoryCaches.cubeMapCount; i++) {
                if (memoryCaches.cubeMapCache[i].cubeMap == texture) {
                    fprintf(file, "(%s,%s,%s,%s,%s,%s)",
                        memoryCaches.cubeMapCache[i].textureName[0]+relativePathLength,
                        memoryCaches.cubeMapCache[i].textureName[1]+relativePathLength,
                        memoryCaches.cubeMapCache[i].textureName[2]+relativePathLength,
                        memoryCaches.cubeMapCache[i].textureName[3]+relativePathLength,
                        memoryCaches.cubeMapCache[i].textureName[4]+relativePathLength,
                        memoryCaches.cubeMapCache[i].textureName[5]+relativePathLength
                    );
                    break;
                }
            }
            }
            break;

        case NODE_VIEWPORT:
            {
            fprintf(file, "v");
            }
            break;

        case NODE_FRAMEBUFFER:
            {
            fprintf(file, "msaa");
            }
            break;

        case NODE_POINT_LIGHT:
            {
            PointLight *pointLight = (PointLight*) node->object;
            fprintf(file, "plight");
            fprintf(file, "(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
                pointLight->ambient[0], pointLight->ambient[1], pointLight->ambient[2], 
                pointLight->diffuse[0], pointLight->diffuse[1], pointLight->diffuse[2], 
                pointLight->specular[0], pointLight->specular[1], pointLight->specular[2],
                pointLight->constant,
                pointLight->linear,
                pointLight->quadratic
            );
            }
            break;

        case NODE_DIRECTIONAL_LIGHT:
            {
            DirectionalLight *directionalLight = (DirectionalLight*) node->object;
            fprintf(file, "dlight");
            fprintf(file, "(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
                directionalLight->ambient[0], directionalLight->ambient[1], directionalLight->ambient[2], 
                directionalLight->diffuse[0], directionalLight->diffuse[1], directionalLight->diffuse[2], 
                directionalLight->specular[0], directionalLight->specular[1], directionalLight->specular[2],
                directionalLight->constant,
                directionalLight->linear,
                directionalLight->quadratic
            );
            }
            break;
        case NODE_SPOT_LIGHT:
            {
            SpotLight *spotLight = (SpotLight*) node->object;
            fprintf(file, "slight");
            fprintf(file, "(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",
                spotLight->ambient[0], spotLight->ambient[1], spotLight->ambient[2], 
                spotLight->diffuse[0], spotLight->diffuse[1], spotLight->diffuse[2], 
                spotLight->specular[0], spotLight->specular[1], spotLight->specular[2],
                spotLight->constant,
                spotLight->linear,
                spotLight->quadratic,
                spotLight->cutOff,
                spotLight->outerCutOff
            );
            }
            break;
    }
}
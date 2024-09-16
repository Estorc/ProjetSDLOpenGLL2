#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
#include "../window.h"
#include "input.h"
#include "../render/camera.h"
#include "shader.h"
#include "../render/depth_map.h"


/**
 * Loads a node from a file and initializes its properties based on the data found in the file.
 * 
 * @param file {FILE*} Pointer to the file from which the node will be loaded.
 * @param c {Camera**} Pointer to a Camera pointer that will be set if a camera node is loaded.
 * @param collisionBuffer {CollisionBuffer*} Pointer to a CollisionBuffer structure for managing collision shapes.
 * 
 * @return {Node*} Returns a pointer to the newly created Node. The node's properties and type 
 *                 are determined by the data read from the file.
 * 
 * This function reads from a file to construct a scene graph node. It handles various types of nodes,
 * including cameras, collision shapes, rigid bodies, static bodies, meshes, models, textured meshes,
 * skyboxes, viewports, and framebuffers. The node's scale, rotation, position, and visibility flags
 * are also configured based on parameters specified in the file. If a camera node is created, it updates
 * the provided Camera pointer. The function also manages the allocation and initialization of related
 * structures and checks for memory allocation failures.
 */

Node *load_node(FILE *file, Camera **c, CollisionBuffer *collisionBuffer, Script scripts[SCRIPTS_COUNT]) {
    
    char symbol[10];
    Node *node;
    
    node = malloc(sizeof(Node));
    POINTER_CHECK(node);
    
    fscanf(file,"%10[a-z]", symbol);
    if (!strcmp(symbol, "empty")) {
        
        create_empty_node(node);
    }
    if (!strcmp(symbol, "camera")) {
        Camera *cam;
        cam = malloc(sizeof(Camera));
        POINTER_CHECK(cam);
        init_camera(cam);
        int active_camera;
        fscanf(file,"(%d)", &active_camera);
        if (active_camera) *c = cam;
        create_camera_node(node, cam);
    }
    if (!strcmp(symbol, "cbox")) {
        
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        fscanf(file,"(%f,%f,%f)\n",
            &boxCollisionShape->size[0], &boxCollisionShape->size[1], &boxCollisionShape->size[2]);
        glm_vec3_scale(boxCollisionShape->size, 0.5f, boxCollisionShape->size);
        create_box_collision_shape_node(node, boxCollisionShape);

    }
    if (!strcmp(symbol, "cplane")) {
        
        PlaneCollisionShape *planeCollisionShape;
        planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
        POINTER_CHECK(planeCollisionShape);
        fscanf(file,"(%f,%f,%f)\n",
            &planeCollisionShape->normal[0], &planeCollisionShape->normal[1], &planeCollisionShape->normal[2]);
        create_plane_collision_shape_node(node, planeCollisionShape);

    }
    if (!strcmp(symbol, "csphere")) {
        
        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);
        fscanf(file,"(%f)\n", &sphereCollisionShape->radius);
        create_sphere_collision_shape_node(node, sphereCollisionShape);

    }
    if (!strcmp(symbol, "kbody")) {
        
        KinematicBody *kinematicBody;
        kinematicBody = malloc(sizeof(KinematicBody));
        kinematicBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(kinematicBody);
        fscanf(file,"(%f,%f,%f,%d)\n", 
            &kinematicBody->velocity[0], &kinematicBody->velocity[1], &kinematicBody->velocity[2], 
            &children_count);
        create_kinematic_body_node(node, kinematicBody);



        kinematicBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        collisionBuffer->length += children_count;
        POINTER_CHECK(kinematicBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, collisionBuffer, scripts, editor);
            kinematicBody->collisionsShapes[kinematicBody->length++] = child;
            child->parent = node;
        }
    }
    if (!strcmp(symbol, "rbody")) {
        
        RigidBody *rigidBody;
        rigidBody = malloc(sizeof(RigidBody));
        rigidBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(rigidBody);
        fscanf(file,"(%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d)\n", 
            &rigidBody->velocity[0], &rigidBody->velocity[1], &rigidBody->velocity[2], 
            &rigidBody->angularVelocity[0], &rigidBody->angularVelocity[1], &rigidBody->angularVelocity[2], 
            &rigidBody->gravity[0], &rigidBody->gravity[1], &rigidBody->gravity[2],
            &rigidBody->mass,
            &rigidBody->friction,
            &rigidBody->centerOfMass[0], &rigidBody->centerOfMass[1], &rigidBody->centerOfMass[2],
            &children_count);
        create_rigid_body_node(node, rigidBody);



        rigidBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        collisionBuffer->length += children_count;
        POINTER_CHECK(rigidBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, collisionBuffer, scripts, editor);
            rigidBody->collisionsShapes[rigidBody->length++] = child;
            child->parent = node;
        }
    }
    if (!strcmp(symbol, "sbody")) {
        
        StaticBody *staticBody;
        staticBody = malloc(sizeof(StaticBody));
        staticBody->length = 0;
        int children_count = 0;
        POINTER_CHECK(staticBody);
        fscanf(file,"(%d)\n", &children_count);
        create_static_body_node(node, staticBody);



        staticBody->collisionsShapes = malloc(sizeof(Node *) * children_count);
        collisionBuffer->length += children_count;
        POINTER_CHECK(staticBody->collisionsShapes);
        
        for (int i = 0; i < children_count; i++) {
            Node *child = load_node(file, c, collisionBuffer, scripts, editor);
            staticBody->collisionsShapes[staticBody->length++] = child;
            child->parent = node;
        }
    }
    if (!strcmp(symbol, "sp")) {
        
        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        create_mesh_node(node, mesh);
    }
    if (!strcmp(symbol, "m")) {
        
        Mesh *mesh;
        mesh = malloc(sizeof(Mesh));
        POINTER_CHECK(mesh);
        create_screen_plane(mesh);
        create_mesh_node(node, mesh);
    }
    if (!strcmp(symbol, "mdl")) {
        
        Model *model;
        model = malloc(sizeof(Model));
        POINTER_CHECK(model);
        char path[100];
        char filename[100];
        fscanf(file,"(%100[^,],%100[^)])", path, filename);
        load_obj_model(path, filename, model);
        create_model_node(node, model);
    }
    if (!strcmp(symbol, "tp")) {
        
        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        char path[100];
        fscanf(file,"(%100[^)])", path);
        create_textured_plane(texturedMesh, path);
        create_textured_mesh_node(node, texturedMesh);
    }
    if (!strcmp(symbol, "sky")) {
        
        TexturedMesh *texturedMesh;
        texturedMesh = malloc(sizeof(TexturedMesh));
        POINTER_CHECK(texturedMesh);
        char path[6][100];
        fscanf(file,"(%100[^,],%100[^,],%100[^,],%100[^,],%100[^,],%100[^)])", path[0],path[1],path[2],path[3],path[4],path[5]);
        create_skybox(texturedMesh, path);
        create_skybox_node(node, texturedMesh);
    }
    if (!strcmp(symbol, "v")) {
        
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
    if (!strcmp(symbol, "msaa")) {
        
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
    
    #ifdef DEBUG
    //print_node(node,0);
    #endif
    char paramSymbol;
    do {
        
        paramSymbol = getc(file);
        
        switch (paramSymbol) {
            case ':':
                
                int children_count = 0;
                fscanf(file,"%d\n", &children_count);
                
                node->children = realloc(node->children, sizeof(Node *) * children_count);
                POINTER_CHECK(node->children);
                
                for (int i = 0; i < children_count; i++)
                    add_child(node, load_node(file, c, collisionBuffer, scripts, editor));
                break;
            case '[':
                char transformSymbol;
                do {
                    
                    transformSymbol = getc(file);
                    
                    float x,y,z;
                    u8 value;
                    switch (transformSymbol) {
                        case 's':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->scale[0] = x;
                            node->scale[1] = y;
                            node->scale[2] = z;
                            break;
                        case 'r':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->rot[0] = x;
                            node->rot[1] = y;
                            node->rot[2] = z;
                            break;
                        case 'm':
                            fscanf(file, "%f,%f,%f", &x, &y, &z);
                            node->pos[0] = x;
                            node->pos[1] = y;
                            node->pos[2] = z;
                            break;
                        case 'v':
                            fscanf(file, "%hhd", &value);
                            if (value)
                                node->flags |= NODE_VISIBLE;
                            else
                                node->flags &= ~NODE_VISIBLE;
                            break;
                    }
                } while (transformSymbol != ']');
                break;
            case '{':
                char scriptname[100];
                fscanf(file, "%100[^}]}", scriptname);
                for (int i = 0; i < SCRIPTS_COUNT; i++) {
                    if (!strcmp(scripts[i].name, scriptname)) {
                        node->flags |= NODE_SCRIPT;
                        node->script = scripts[i].script;
                    }
                        
                }
                break;
        }
    } while (paramSymbol != '\n' && paramSymbol != ':' && paramSymbol != -1);
    
    return node;
}

/**
 * Loads a scene from a specified file path and initializes a scene graph structure.
 * 
 * @param path {char*} - Path to the scene file to be loaded.
 * @param c {Camera**} - Pointer to a Camera pointer that will be set if a camera node is loaded.
 * @param collisionBuffer {CollisionBuffer*} - Pointer to a CollisionBuffer structure for managing collision shapes.
 * 
 * @return Node* Returns a pointer to the root Node of the loaded scene graph.
 *               If the file cannot be opened, returns NULL.
 * 
 * This function opens a scene file specified by the `path` parameter, reads the contents, 
 * and constructs a scene graph using the `load_node` function. The scene graph's root node
 * is created first, and then it creates a viewport node that holds the reference to the root scene.
 * The function also allocates memory for collision shapes based on the number of shapes encountered 
 * during the loading process. It prints the structure of the loaded nodes for debugging purposes.
 * If any allocation fails during the loading process, appropriate error handling should be considered.
 */

Node *load_scene(char *path, Camera **c, CollisionBuffer *collisionBuffer, Script *scripts) {
    collisionBuffer->length = 0;
    FILE * file = fopen(path, "r");

    if (!file) return NULL;

    Node *root;
    root = load_node(file, c, collisionBuffer, scripts, 0);
    if (!root) {
        fclose(file);
        return NULL;
    }
    print_node(root, 0);

    Node *node;
    node = load_node(file, c, collisionBuffer, scripts, 0);
    if (!node) {
        free_node(root);
        fclose(file);
        return NULL;
    }

    Viewport *viewport = (Viewport *) node->object;
    viewport->scene = root;
    root->parent = node;
    node->parent = NULL;
    print_node(node, 0);
    collisionBuffer->collisionsShapes = malloc(sizeof(Node *) * collisionBuffer->length);
    if (!collisionBuffer->collisionsShapes) {
        free_node(root);
        free_node(node);
        fclose(file);
        return NULL;
    }

    fclose(file);

    return node;

}
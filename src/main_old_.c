#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "window.h"
#include "io/input.h"
#include "render/camera.h"
#include "io/shader.h"
#include "utils/skybox.h"
#include "render/depth_map.h"

int update(Window *window, Node *viewportNode, Camera *c, Input *input, Shader shaders[], DepthMap *depthMap) {

    if (update_input(c, input) == -1) return -1;
    update_window(window, viewportNode, c, shaders, depthMap);

    return 0;

}

Node *load_test() {
    Model *model;
    model = malloc(sizeof(Model));
    load_obj_model("assets/models/dillonPonchoHDMdl.vpl/", "untitled.obj", model);
    Node *model2;
    model2 = malloc(sizeof(Node));
    return create_model_node(model2, model);
}

const int NUM_MODELS = 2;
const int NUM_SHADERS = 4;
const int NUM_TEXTURED_MESHES = 2;
const int NUM_MESHES = 1;
const int NUM_NODES = 10;

int main(int argc, char *argv[]) {

    Window window;
    if (create_window("OPENGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL, &window) == -1) return -1;
    
    Input input;
    input.keys = 0;

    Camera cam;
    init_camera(&cam);


    Shader shaders[NUM_SHADERS];
    create_shaders(shaders);


    Model models[NUM_MODELS];
    if (load_obj_model("assets/models/Deku Tree/", "untitled.obj", &models[0]) == -1) return -1;
    Vec3fScale(models[0].scale, 0.01f);


    DepthMap depthMap;
    create_depthmap(&depthMap);

    Mesh meshes[NUM_MESHES];
    create_screen_plane(&meshes[0]);

    TexturedMesh texturedMeshes[NUM_TEXTURED_MESHES];
    create_skybox(&texturedMeshes[0], SKYBOX_SEA);
    create_textured_plane(&texturedMeshes[1], "assets/models/texture.png");
    texturedMeshes[1].rot[0] = 180.0f;

    FrameBuffer msaa_framebuffer;
    FrameBuffer msaa_framebuffer_intermediate;
    Filter msaa_framebuffer_filter;
    msaa_framebuffer_filter.beginScript = anti_aliasing_begin_script;
    msaa_framebuffer_filter.endScript = anti_aliasing_end_script;
    msaa_framebuffer_filter.flags = FILTER_DEFAULT_FLAGS;
    create_msaa_framebuffer(&msaa_framebuffer, &msaa_framebuffer_intermediate, &msaa_framebuffer_filter.texture);




    Node root;
    create_mesh_node(&root, &meshes[0]);

    printf("Created root!\n");

    root.children = realloc(root.children, sizeof(Node *) * 4);
    POINTER_CHECK(root.children);

    printf("Malloc children!\n");

    Node model1;
    add_child(&root, create_model_node(&model1, &models[0]));
    add_child(&root,load_test());
    Node texturedMesh2;
    add_child(&root, create_textured_mesh_node(&texturedMesh2, &texturedMeshes[1]));

    Node skybox;
    add_child(&root, create_skybox_node(&skybox, &texturedMeshes[0]));

    printf("Added children!\n");

    print_node(&root, 0);




    Node viewportNode;
    Viewport viewport;
    create_viewport_node(&viewportNode, &viewport);
    viewport.scene = &root;

    printf("Created viewport!\n");

    viewportNode.children = realloc(viewportNode.children, sizeof(Node *) * 1);
    POINTER_CHECK(viewportNode.children);

    printf("Malloc children!\n");


    Node frameBufferMSAATexture;
    add_child(&viewportNode, create_filter_node(&frameBufferMSAATexture, &msaa_framebuffer_filter));

    frameBufferMSAATexture.children = realloc(frameBufferMSAATexture.children, sizeof(Node *) * 2);
    POINTER_CHECK(frameBufferMSAATexture.children);

    Node frameBufferMSAA;
    add_child(&frameBufferMSAATexture, create_framebuffer_node(&frameBufferMSAA, &msaa_framebuffer));
    Node frameBufferMSAAIntermediate;
    add_child(&frameBufferMSAATexture, create_framebuffer_node(&frameBufferMSAAIntermediate, &msaa_framebuffer_intermediate));


    printf("Added children!\n");

    print_node(&viewportNode, 0);

    while (update(&window, &viewportNode, &cam, &input, shaders, &depthMap) >= 0);

    free_node(&root);
    free_node(&viewportNode);

    printf("Free node!\n");

}




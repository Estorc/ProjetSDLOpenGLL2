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

int update(Window *window, Node *nodes, Camera *c, Input *input, Shader shaders[], DepthMap *depthMap) {

    if (update_input(c, input, nodes) == -1) return -1;
    update_window(window, nodes, c, shaders, depthMap);

    return 0;

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
    if (load_obj_model("assets/models/dillonPonchoHDMdl.vpl/", "untitled.obj", &models[1]) == -1) return -1;
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
    TextureMap msaa_framebuffer_texture;
    create_msaa_framebuffer(&msaa_framebuffer, &msaa_framebuffer_intermediate, &msaa_framebuffer_texture);

    Node nodes[NUM_NODES];
    create_mesh_node(&nodes[SCREEN_MESH], &meshes[0]);
    create_model_node(&nodes[MODEL_1], &models[0]);
    create_model_node(&nodes[MODEL_2], &models[1]);
    create_textured_mesh_node(&nodes[TEXTURED_MESH_1], &texturedMeshes[0]);
    create_textured_mesh_node(&nodes[TEXTURED_MESH_2], &texturedMeshes[1]);
    create_framebuffer_node(&nodes[FRAMEBUFFER_MSAA], &msaa_framebuffer);
    create_framebuffer_node(&nodes[FRAMEBUFFER_MSAA_INTERMEDIATE], &msaa_framebuffer_intermediate);
    create_framebuffer_node(&nodes[FRAMEBUFFER_MSAA_TEXTURE], &msaa_framebuffer_texture);

    while (update(&window, nodes, &cam, &input, shaders, &depthMap) >= 0);

}




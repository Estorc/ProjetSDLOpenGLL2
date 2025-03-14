MODULES += src/math/math_util.o

MODULES += src/classes/classes.o

MODULES += src/io/socket/client.o
MODULES += src/io/socket/server.o
MODULES += src/io/socket/network_utils.o

ifneq ($(OS),Windows_NT)
MODULES += src/io/model_loaders/fbx_loader.o
endif
MODULES += src/io/model_loaders/obj_loader.o
MODULES += src/io/model_loaders/mtl_loader.o
MODULES += src/io/model_loaders/model_loader.o

MODULES += src/io/gltexture_loader.o
MODULES += src/io/scene_loader.o
MODULES += src/io/node_loader.o
MODULES += src/io/model.o
MODULES += src/io/input.o
MODULES += src/io/stringio.o
MODULES += src/io/shader.o
MODULES += src/io/osio.o
MODULES += src/io/audio.o

MODULES += src/render/lighting.o
MODULES += src/render/render.o
MODULES += src/render/camera.o
MODULES += src/render/framebuffer.o
MODULES += src/render/depth_map.o

MODULES += src/physics/physics.o
MODULES += src/physics/collision_util.o
MODULES += src/physics/collision.o

MODULES += src/utils/random.o
MODULES += src/utils/scene.o
MODULES += src/utils/time.o

MODULES += src/gui/frame.o

MODULES += src/storage/stack.o
MODULES += src/storage/queue.o
MODULES += src/storage/hash.o

MODULES += src/scripts/scripts.o

MODULES += src/term/term.o

MODULES += src/raptiquax.o
MODULES += src/memory.o
MODULES += src/buffer.o
MODULES += src/window.o
MODULES += src/settings.o

MODULES += src/main.o
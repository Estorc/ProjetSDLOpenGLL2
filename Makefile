# ===============================================================
# ====================     Makefile     =========================
# ===============================================================

newline := $(shell printf "\n")

# C Files path

modules += src/io/gltexture_loader.c
modules += src/io/obj_loader.c
modules += src/io/mtl_loader.c
modules += src/io/scene_loader.c
modules += src/io/node_loader.c
modules += src/io/model.c
modules += src/io/input.c
modules += src/io/stringio.c
modules += src/io/shader.c
modules += src/io/osio.c

modules += src/math/math_util.c
modules += src/math/graph.c

modules += src/render/lighting.c
modules += src/render/render.c
modules += src/render/color.c
modules += src/render/camera.c
modules += src/render/framebuffer.c
modules += src/render/depth_map.c
modules += src/render/viewport.c
modules += src/render/filter.c

modules += src/physics/cube.c
modules += src/physics/collision.c
modules += src/physics/bodies.c

modules += src/utils/skybox.c
modules += src/utils/time.c

modules += src/memory.c
modules += src/window.c
modules += src/node.c

# ===============================================================

# Flags

LFLAGS += `sdl2-config --cflags --libs`
LFLAGS += -lm
LFLAGS += -Ilib
LFLAGS += -lSDL2_image
LFLAGS += -lGL
LFLAGS += -lGLU
LFLAGS += -lGLX
LFLAGS += -lSDL2_ttf

WFLAGS += -Wall
WFLAGS += -Wno-implicit-function-declaration


LOADING_SCRIPT_HEADER := src/scripts/loading_scripts.h
SCRIPTS_FILES := $(notdir $(wildcard src/scripts/*.cscript))
SCRIPTS_COUNT := -D SCRIPTS_COUNT=$(words $(SCRIPTS_FILES))

NODE_TOOLS_HEADER := src/utils/node_tools.h
NODE_TOOLS := $(NODE_TOOLS)$(shell tools/node_tools 0)\n
NODE_TOOLS := $(NODE_TOOLS)$(shell tools/node_tools 1)

# ===============================================================

# Targets

all:
	@echo "===================== Begin build. ===================="
	@$(MAKE) generate_header
	@echo "Build app..."
	@gcc -o app src/main.c ${SCRIPTS_COUNT} ${modules} ${LFLAGS} ${WFLAGS}
	@echo "============= Successfully build the app! ============="

debug:
	@echo "===================== Begin debug build. ===================="
	@$(MAKE) generate_header
	@echo "Build debug app..."
	@gcc -o debug -g src/main.c -DDEBUG ${SCRIPTS_COUNT} ${modules} ${LFLAGS} ${WFLAGS}
	@echo "============= Successfully build the debug app! ============="

bin:
	@echo "===================== Begin build. ===================="
	@$(MAKE) generate_header
	@echo "Build app..."
	@gcc -o bin/release/app src/main.c ${SCRIPTS_COUNT} ${modules} ${LFLAGS} ${WFLAGS}
	@echo "============= Successfully build the app! ============="

tools:
	@echo "===================== Begin build tools. ===================="
	@echo "Build app..."
	@gcc -o tools/node_tools tools/node_tools.c ${WFLAGS}
	@echo "============= Successfully build the tools! ============="
	@$(MAKE) generate_header

%:
	@if test ! -f src/$@.c; then echo "File $@.c doesn't exist in src/"; exit 1; fi
	@echo "===================== Begin build. ===================="
	@$(MAKE) generate_header
	@echo "Build $@..."
	@gcc -o $@ src/$@.c ${SCRIPTS_COUNT} ${modules} ${LFLAGS} ${WFLAGS}
	@echo "============= Successfully build $@! ============="


generate_header:
	@echo "Generate loading scripts header..."
	@echo "// Auto-generated scripts loading header file" > $(LOADING_SCRIPT_HEADER)
	@for file in $(SCRIPTS_FILES); do \
		echo "#include \"$$file\"" >> $(LOADING_SCRIPT_HEADER); \
	done

	@echo "Generate node tools header..."
	@echo "// Auto-generated node tools header file" > $(NODE_TOOLS_HEADER)
	@echo "$(NODE_TOOLS)" >> $(NODE_TOOLS_HEADER)

.PHONY: all debug bin tools generate_header
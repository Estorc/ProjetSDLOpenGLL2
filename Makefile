# ===============================================================
# ====================     Makefile     =========================
# ===============================================================


NC=\033[0;0m

BOLD=1
ITALIC=3
UNDELINE=4
BLINK=5

FG_GREY=30
FG_RED=31
FG_GREEN=32
FG_ORANGE=33
FG_BLUE=34
FG_PURPLE=35
FG_AQUA=36
FG_WHITE=37
FG_DEFAULT=39

BG_GREY=40
BG_RED=41
BG_GREEN=42
BG_ORANGE=43
BG_BLUE=44
BG_PURPLE=45
BG_AQUA=46
BG_WHITE=47
BG_DEFAULT=49

NOTE_COL=\033[0;${FG_GREY};${BG_DEFAULT}m
ACT_COL=\033[0;${FG_BLUE};${BG_DEFAULT}m
STEP_COL=\033[0;${FG_DEFAULT};${BG_DEFAULT};${BOLD}m
FILE_COL=\033[0;${FG_ORANGE};${BG_DEFAULT};${ITALIC}m
SUCCESS_COL=\033[0;${FG_GREEN};${BG_DEFAULT};${BOLD}m

NEWLINE := $(shell printf "\n")

# Object Files path

BUILD_DIR := bin
PROCESSED_CLASS_DIR := src/classes/__processed__

MODULES += src/io/gltexture_loader.o
MODULES += src/io/obj_loader.o
MODULES += src/io/mtl_loader.o
MODULES += src/io/scene_loader.o
MODULES += src/io/node_loader.o
MODULES += src/io/model.o
MODULES += src/io/input.o
MODULES += src/io/stringio.o
MODULES += src/io/shader.o
MODULES += src/io/osio.o

MODULES += src/math/math_util.o
MODULES += src/math/graph.o

MODULES += src/render/lighting.o
MODULES += src/render/render.o
MODULES += src/render/color.o
MODULES += src/render/camera.o
MODULES += src/render/framebuffer.o
MODULES += src/render/depth_map.o

MODULES += src/physics/physics.o
MODULES += src/physics/collision_util.o
MODULES += src/physics/isolate.o
MODULES += src/physics/collision.o
MODULES += src/physics/bodies.o

MODULES += src/utils/skybox.o
MODULES += src/utils/time.o

MODULES += src/gui/frame.o

MODULES += src/storage/node.o
MODULES += src/storage/stack.o
MODULES += src/storage/queue.o

MODULES += src/memory.o
MODULES += src/buffer.o
MODULES += src/window.o
MODULES += src/settings.o
MODULES += src/main.o

RELEASE_MODULES = $(addprefix $(BUILD_DIR)/,${MODULES})
DEBUG_MODULES = $(addprefix $(BUILD_DIR)/debug/,${MODULES})

# ===============================================================

# Flags

CFLAGS += -MD

LFLAGS += `sdl2-config --cflags --libs`
LFLAGS += -lm
LFLAGS += -Ilib
LFLAGS += -lSDL2_image
LFLAGS += -lSDL2_ttf
LFLAGS += -lSDL2_mixer
LFLAGS += -lGL
LFLAGS += -lGLU
LFLAGS += -lGLX

WFLAGS += -Wall
WFLAGS += -Wno-implicit-function-declaration


LOADING_SCRIPT_HEADER := src/scripts/loading_scripts.h
NEW_SCRIPT_SYMBOL := NEW_SCRIPT
SCRIPTS_PATHS := $(wildcard src/scripts/*.cscript)
SCRIPTS_FILES := $(notdir $(SCRIPTS_PATHS))
SCRIPTS_COUNT := -D SCRIPTS_COUNT=$(shell grep -o '\b$(NEW_SCRIPT_SYMBOL)\b' $(SCRIPTS_PATHS) | wc -l)

CLASS_TOOLS := $(shell tools/class_tools)
CLASSES_MODULES := $(wildcard src/classes/__processed__/*.class.c)
CLASSES_MODULES := $(patsubst %.class.c, %.class.o, $(CLASSES_MODULES))

RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(BUILD_DIR)/,${CLASSES_MODULES})
DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(BUILD_DIR)/debug/,${CLASSES_MODULES})

# ===============================================================

# Targets

all:release launch

init_build:
	@echo "${STEP_COL}===================== Begin build. ====================${NC}"

launch:
	@echo "${STEP_COL}=================== Launch the app... =================${NC}"
	@${BUILD_DIR}/release/app


release: generate_header init_build ${RELEASE_MODULES}
	@echo "${STEP_COL}===================== Begin linking. ====================${NC}"
	@echo "${ACT_COL}Linking app...${NC}"
	@mkdir -p ${BUILD_DIR}/release
	@gcc -o ${BUILD_DIR}/release/app ${SCRIPTS_COUNT} ${RELEASE_MODULES} ${LFLAGS} ${WFLAGS}

	@echo "${ACT_COL}Copying assets...${NC}"
	@rsync -rupE assets ${BUILD_DIR}/release/
	@echo "${SUCCESS_COL}Assets copied!${NC}"
	@echo "${ACT_COL}Copying shaders...${NC}"
	@rsync -rupE shaders ${BUILD_DIR}/release/
	@echo "${SUCCESS_COL}Shaders copied!${NC}"

	@echo "${STEP_COL}============= ${SUCCESS_COL}Successfully build the app!${NC}${STEP_COL} =============${NC}"

debug: generate_header init_build ${DEBUG_MODULES}
	@echo "${STEP_COL}===================== Begin debug linking. ====================${NC}"
	@echo "${ACT_COL}Linking debug app...${NC}"
	@mkdir -p ${BUILD_DIR}/debug
	@gcc -o ${BUILD_DIR}/debug/app -g -O0 ${SCRIPTS_COUNT} ${DEBUG_MODULES} ${LFLAGS} ${WFLAGS}

	@echo "${ACT_COL}Copying assets...${NC}"
	@rsync -rupE assets ${BUILD_DIR}/debug/
	@echo "${SUCCESS_COL}Assets copied!${NC}"
	@echo "${ACT_COL}Copying shaders...${NC}"
	@rsync -rupE shaders ${BUILD_DIR}/debug/
	@echo "${SUCCESS_COL}Shaders copied!${NC}"

	@echo "${STEP_COL}============= ${SUCCESS_COL}Successfully build the debug app!${NC}${STEP_COL} =============${NC}"

tools:
	@echo "${STEP_COL}===================== Begin build tools. ===================="
	@echo "${ACT_COL}Build tools...${NC}"
	@gcc -o tools/class_tools tools/class_tools.c ${WFLAGS} -Wno-format-truncation
	@gcc -o tools/node_tools tools/node_tools.c ${WFLAGS}
	@echo "${STEP_COL}============= ${NC}${SUCCESS_COL}Successfully build the tools!${NC}${STEP_COL} =============${NC}"

# Release objects constructor
${BUILD_DIR}/%.o: %.c
	@echo "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}..."
	@mkdir -p ${BUILD_DIR}/${dir $*}
	@gcc -c $*.c -o ${BUILD_DIR}/$*.o ${CFLAGS} ${SCRIPTS_COUNT} ${LFLAGS} ${WFLAGS}
	@echo "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${BUILD_DIR}/$*.o${NC}"

# Debug objects constructor
${BUILD_DIR}/debug/%.o: %.c
	@echo "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}..."
	@mkdir -p ${BUILD_DIR}/debug/${dir $*}
	@gcc -c $*.c -g -o ${BUILD_DIR}/debug/$*.o -DDEBUG ${CFLAGS} ${SCRIPTS_COUNT} ${LFLAGS} ${WFLAGS}
	@echo "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${BUILD_DIR}/debug/$*.o${NC}"

generate_header:
	@echo "Generate loading scripts header..."
	@echo "// Auto-generated scripts loading header file" > $(LOADING_SCRIPT_HEADER)
	@for file in $(SCRIPTS_FILES); do \
		echo "#include \"$$file\"" >> $(LOADING_SCRIPT_HEADER); \
	done

clean:
	@echo "${ACT_COL}Clear the build...${NC}"
	@rm -rf ${PROCESSED_CLASS_DIR}
	@rm -rf ${BUILD_DIR}
	@echo "${SUCCESS_COL}Successfully clear the build!${NC}"

.PHONY: all build debug tools generate_header
-include $(RELEASE_MODULES:.o=.d)
-include $(DEBUG_MODULES:.o=.d)
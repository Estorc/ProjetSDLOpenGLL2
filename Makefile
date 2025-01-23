# ===============================================================
# ====================     Makefile     =========================
# ===============================================================

export TERM = xterm-256color

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

PRINT=@printf

NEWLINE := $(shell printf "\n")

# Object Files path

BUILD_DIR := bin
TEST_DIR := test
PROCESSED_CLASS_DIR := src/__processed__

LIBS += lib/ufbx/ufbx.o

MODULES += src/classes/classes.o

MODULES += src/io/model_loaders/obj_loader.o
MODULES += src/io/model_loaders/fbx_loader.o
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

MODULES += src/math/math_util.o

MODULES += src/render/lighting.o
MODULES += src/render/render.o
MODULES += src/render/camera.o
MODULES += src/render/framebuffer.o
MODULES += src/render/depth_map.o

MODULES += src/physics/physics.o
MODULES += src/physics/collision_util.o
MODULES += src/physics/collision.o

MODULES += src/utils/scene.o
MODULES += src/utils/time.o

MODULES += src/gui/frame.o

MODULES += src/storage/stack.o
MODULES += src/storage/queue.o

MODULES += src/raptiquax.o
MODULES += src/memory.o
MODULES += src/buffer.o
MODULES += src/window.o
MODULES += src/settings.o

MAIN=src/main.o

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
CLASSES_MODULES := $(wildcard $(PROCESSED_CLASS_DIR)/*.class.c)
CLASSES_MODULES := $(patsubst %.class.c, %.class.o, $(CLASSES_MODULES))

RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(BUILD_DIR)/,${CLASSES_MODULES})
DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(BUILD_DIR)/debug/,${CLASSES_MODULES})

GCC = gcc

# ===============================================================

# Targets

all:release launch

init_build:
	@printf "${STEP_COL}===================== Begin build. ====================${NC}\n"

launch:
	@printf "${STEP_COL}=================== Launch the app... =================${NC}\n"
	@${BUILD_DIR}/release/app


release: generate_header init_build ${RELEASE_MODULES} $(BUILD_DIR)/${MAIN} ${LIBS}
	@printf "${STEP_COL}===================== Begin linking. ====================${NC}\n"
	@printf "${ACT_COL}Linking app...${NC}\n"
	@mkdir -p ${BUILD_DIR}/release
	@${GCC} -o ${BUILD_DIR}/release/app ${SCRIPTS_COUNT} ${RELEASE_MODULES} $(BUILD_DIR)/${MAIN} ${LIBS} ${LFLAGS} ${WFLAGS}

	@printf "${ACT_COL}Copying assets...${NC}\n"
	@rsync -rupE assets ${BUILD_DIR}/release/
	@printf "${SUCCESS_COL}Assets copied!${NC}\n"
	@printf "${ACT_COL}Copying shaders...${NC}\n"
	@rsync -rupE shaders ${BUILD_DIR}/release/
	@printf "${SUCCESS_COL}Shaders copied!${NC}\n"

	@printf "${STEP_COL}============= ${SUCCESS_COL}Successfully build the app!${NC}${STEP_COL} =============${NC}\n"

debug: generate_header init_build ${DEBUG_MODULES} $(BUILD_DIR)/debug/${MAIN} ${LIBS}
	@printf "${STEP_COL}===================== Begin debug linking. ====================${NC}\n"
	@printf "${ACT_COL}Linking debug app...${NC}\n"
	@mkdir -p ${BUILD_DIR}/debug
	@${GCC} -o ${BUILD_DIR}/debug/app -g -O0 ${SCRIPTS_COUNT} ${DEBUG_MODULES} $(BUILD_DIR)/debug/${MAIN} ${LIBS} ${LFLAGS} ${WFLAGS}

	@printf "${ACT_COL}Copying assets...${NC}\n"
	@rsync -rupE assets ${BUILD_DIR}/debug/
	@printf "${SUCCESS_COL}Assets copied!${NC}\n"
	@printf "${ACT_COL}Copying shaders...${NC}\n"
	@rsync -rupE shaders ${BUILD_DIR}/debug/
	@printf "${SUCCESS_COL}Shaders copied!${NC}\n"

	@printf "${STEP_COL}============= ${SUCCESS_COL}Successfully build the debug app!${NC}${STEP_COL} =============${NC}\n"

tools:
	@printf "${STEP_COL}===================== Begin build tools. ====================\n"
	@printf "${ACT_COL}Build tools...${NC}\n"
	@${GCC} -o tools/class_tools tools/class_tools.c ${WFLAGS} -Wno-format-truncation
	@${GCC} -o tools/node_tools tools/node_tools.c ${WFLAGS}
	@printf "${STEP_COL}============= ${NC}${SUCCESS_COL}Successfully build the tools!${NC}${STEP_COL} =============${NC}\n"


${TEST_DIR}/%: ${TEST_DIR}/%.o ${DEBUG_MODULES} ${LIBS}
	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@${GCC} -g -O0 -o $@ $^ ${LFLAGS} ${WFLAGS}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}$@${NC}\n"

${TEST_DIR}/%.o: ${TEST_DIR}/%.c
	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@${GCC} -c $< -o $@ ${CFLAGS} ${LFLAGS} ${WFLAGS}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}$@${NC}\n"


%.o: %.c
	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@${GCC} -c $< -o $@ ${CFLAGS} ${LFLAGS} ${WFLAGS}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}$@${NC}\n"

# Release objects constructor
${BUILD_DIR}/%.o: %.c
	@printf "${ACT_COL}Preprocessing ${FILE_COL}\"$<\"${NC}...\n"
	@mkdir -p ${PROCESSED_CLASS_DIR}/${dir $<}
	@python3 ./tools/preprocessor_pipeline.py $< ${PROCESSED_CLASS_DIR}/${dir $<}

	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@mkdir -p ${BUILD_DIR}/${dir $*}
	@${GCC} -c ${PROCESSED_CLASS_DIR}/$< -o ${BUILD_DIR}/$*.o -I$(dir $*) ${CFLAGS} ${SCRIPTS_COUNT} ${LFLAGS} ${WFLAGS}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${BUILD_DIR}/$*.o${NC}\n"

# Debug objects constructor
${BUILD_DIR}/debug/%.o: %.c
	@printf "${ACT_COL}Preprocessing ${FILE_COL}\"$<\"${NC}...\n"
	@mkdir -p ${PROCESSED_CLASS_DIR}/${dir $<}
	@python3 ./tools/preprocessor_pipeline.py $< ${PROCESSED_CLASS_DIR}/${dir $<}

	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@mkdir -p ${BUILD_DIR}/debug/${dir $*}
	@${GCC} -c ${PROCESSED_CLASS_DIR}/$< -g -o ${BUILD_DIR}/debug/$*.o -I$(dir $*) -DDEBUG ${CFLAGS} ${SCRIPTS_COUNT} ${LFLAGS} ${WFLAGS}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${BUILD_DIR}/debug/$*.o${NC}\n"

# @python3 ./tools/preprocessor_pipeline.py $$file/${dir $<}
generate_header:
	@printf "Generate loading scripts header...\n"
	@echo "// Auto-generated scripts loading header file" > $(LOADING_SCRIPT_HEADER)
	@for file in $(SCRIPTS_PATHS); do \
		python3 ./tools/preprocessor_pipeline.py $$file ${PROCESSED_CLASS_DIR}/$$(dirname $$file); \
		echo "#include \"../__processed__/$$file\"" >> $(LOADING_SCRIPT_HEADER); \
	done

clean:
	@printf "${ACT_COL}Clear the build...${NC}\n"
	@rm -rf ${PROCESSED_CLASS_DIR}
	@rm -rf ${BUILD_DIR}
	@printf "${SUCCESS_COL}Successfully clear the build!${NC}\n"

.PHONY: all build debug tools generate_header
-include $(BUILD_DIR)/$(MAIN:.o=.d)
-include $(BUILD_DIR)/debug/$(MAIN:.o=.d)
-include $(RELEASE_MODULES:.o=.d)
-include $(DEBUG_MODULES:.o=.d)
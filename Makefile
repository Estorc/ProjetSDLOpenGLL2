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

PYTHON = python3
PIP = $(shell which pip)
ifeq ($(PIP),)
	HAVE_PIP = 0
else
	HAVE_PIP = 1
endif
PYTHON_REQUIREMENTS = ./lib/python_requirements.txt

KEEP_TEMP_FILES = 0

BUILD_DIR := bin
OBJ_DIR := obj
TEST_DIR := test

RELEASE_DIR := release
DEBUG_DIR := debug

PROCESSED_CLASS_DIR := src/__processed__

ifneq ($(OS),Windows_NT)
LIBS += lib/ufbx/ufbx.o
endif

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

MAIN=src/main.o
APP_NAME=app

RELEASE_MODULES = $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/,${MODULES})
DEBUG_MODULES = $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/,${MODULES})


# ===============================================================

# Flags

CFLAGS += -MD

LFLAGS += `sdl2-config --cflags --libs`
LFLAGS += -lm
LFLAGS += -Ilib
LFLAGS += -lSDL2_image
LFLAGS += -lSDL2_ttf
LFLAGS += -lSDL2_mixer
LFLAGS += -pthread
ifeq ($(OS),Windows_NT)
LFLAGS += -lglew32
LFLAGS += -lopengl32
LFLAGS += -lavif
LFLAGS += -lws2_32
LFLAGS += -mconsole
else
LFLAGS += -lGL
LFLAGS += -lGLU
LFLAGS += -lGLX
endif

WFLAGS += -Wall
WFLAGS += -Wno-implicit-function-declaration


LOADING_SCRIPT_HEADER := src/scripts/loading_scripts.h
NEW_SCRIPT_SYMBOL := NEW_SCRIPT
SCRIPTS_PATHS := $(shell find src/scripts -type f -name "*.cscript")
SCRIPTS_MODULES := $(SCRIPTS_PATHS:.cscript=.o)

RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/,${SCRIPTS_MODULES})
DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/,${SCRIPTS_MODULES})

ifeq ($(OS),Windows_NT) 
CLASS_TOOLS := $(shell tools/class_tools.exe)
else
CLASS_TOOLS := $(shell tools/class_tools)
endif
CLASSES_MODULES := $(wildcard $(PROCESSED_CLASS_DIR)/*.class.c)
CLASSES_MODULES := $(patsubst %.class.c, %.class.o, $(CLASSES_MODULES))

RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/,${CLASSES_MODULES})
DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/,${CLASSES_MODULES})

GCC = gcc

# ===============================================================

# Targets

define init_build
	@printf "${STEP_COL}===================== Begin build. ====================${NC}\n"
endef

define launch
	@printf "${STEP_COL}=================== Launch ${1}... =================${NC}\n"
	@${1}
endef

define build_executable
	@mkdir -p ${1}
	@printf "${ACT_COL}Linking ${FILE_COL}\"${APP_NAME}\"${NC}...\n"
	@${GCC} -o ${1}/${APP_NAME} $^ ${LFLAGS} ${WFLAGS} ${2}

	@if [ ${3} = "copy_assets" ]; then \
		printf "${ACT_COL}Copying assets...${NC}\n"; \
		rsync -rupE assets ${1}/; \
		printf "${SUCCESS_COL}Assets copied!${NC}\n"; \
		printf "${ACT_COL}Copying shaders...${NC}\n"; \
		rsync -rupE shaders ${1}/; \
		printf "${SUCCESS_COL}Shaders copied!${NC}\n"; \
	fi

	@printf "${STEP_COL}============= ${SUCCESS_COL}Build successful!${NC}${STEP_COL} =============${NC}\n"
endef

# Function to handle common preprocessing and compilation
define build_object
	@printf "${ACT_COL}Preprocessing ${FILE_COL}\"$<\"${NC} => ${FILE_COL}\"${dir $<}~${notdir $<}\"${NC}...\n"
	@$(PYTHON) ./tools/preprocessor_pipeline.py $< ${dir $<}

	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@mkdir -p ${OBJ_DIR}/${1}/${dir $*}
	@${GCC} -x c -c ${PROCESSED_CLASS_DIR}/$< -o ${OBJ_DIR}/${1}/$*.o -I$(dir $*) ${DFLAGS} ${CFLAGS} ${LFLAGS} ${WFLAGS} ${2}
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${OBJ_DIR}/${1}/$*.o${NC}\n"

	@if [ KEEP_TEMP_FILES = 0 ]; then \
		rm -rf ${dir $<}~${notdir $<}; \
	fi
endef

all:release

release: ${RELEASE_MODULES} $(OBJ_DIR)/$(RELEASE_DIR)/${MAIN} ${LIBS}
	$(call init_build)
	$(call build_executable, ${BUILD_DIR}/$(RELEASE_DIR), ,copy_assets)
	$(call launch, ${BUILD_DIR}/$(RELEASE_DIR)/$(APP_NAME))

debug: ${DEBUG_MODULES} $(OBJ_DIR)/$(DEBUG_DIR)/${MAIN} ${LIBS}
	$(call init_build)
	$(call build_executable, ${BUILD_DIR}/$(DEBUG_DIR), -g -O0,copy_assets)

${TEST_DIR}/%: ${TEST_DIR}/%.o ${DEBUG_MODULES} ${LIBS}
	$(call build_object, ${OBJ_DIR}/$(TEST_DIR), , )


# === Objects constructors ===


%.o: %.c
	$(call build_executable,,, )

${OBJ_DIR}/${TEST_DIR}/%.o: %.c
	$(call build_executable, $(TEST_DIR), -g -O0, )

# Release objects constructor for .c files
${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.c
	$(call build_object,$(RELEASE_DIR), )

# Debug objects constructor for .c files
${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.c
	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

# Release objects constructor for .cscript files
${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.cscript
	$(call build_object,$(RELEASE_DIR), )

# Debug objects constructor for .cscript files
${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.cscript
	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

tools:
	@printf "${STEP_COL}===================== Begin build tools. ====================\n"
	@printf "${ACT_COL}Build tools...${NC}\n"
	@${GCC} -o tools/class_tools tools/class_tools.c ${DFLAGS} ${WFLAGS} -Wno-format
	@printf "${STEP_COL}============= ${SUCCESS_COL}Successfully build the tools!${STEP_COL} =============${NC}\n"

clean:
	@printf "${ACT_COL}Clear the build...${NC}\n"
	@rm -rf ${PROCESSED_CLASS_DIR}
	@rm -rf ${BUILD_DIR}
	@rm -rf ${OBJ_DIR}
	@printf "${SUCCESS_COL}Successfully clear the build!${NC}\n"


# Default target to install libraries
install: check-python
	@printf "${ACT_COL}Install python requirements...${NC}\n"
	@$(PIP) install -r $(PYTHON_REQUIREMENTS)
	@printf "${SUCCESS_COL}Successfully install python requirements!${NC}\n"

# Ensure that the environment is not externally-managed
check-python:
	@if [ $(HAVE_PIP) = "0" ]; then \
		echo "Error: pip is not installed. Please install pip."; \
		exit 1; \
	elif [ -f "/usr/lib/python$(shell $(PYTHON) -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')" ]; then \
		echo "Warning: You're using a system-managed Python environment. Installing packages globally may cause conflicts."; \
		echo "Consider using a virtual environment for your project."; \
		exit 1; \
	fi

docs:
	@printf "${ACT_COL}Generate documentation...${NC}\n"
	@doxygen Doxyfile
	@printf "${SUCCESS_COL}Documentation generated!${NC}\n"



# Ensure that header files are ignored to prevent errors
%.h:
	@printf "${ACT_COL}Ignoring $@${NC}\n"



.PHONY: all build debug tools install clean docs
-include $(BUILD_DIR)/$(MAIN:.o=.d)
-include $(BUILD_DIR)/debug/$(MAIN:.o=.d)
-include $(RELEASE_MODULES:.o=.d)
-include $(DEBUG_MODULES:.o=.d)
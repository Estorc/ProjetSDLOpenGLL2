CC = gcc

# Dossiers
INTRO_SRC_DIR = intro-game/src
INTRO_INCLUDE_DIR = intro-game/include
ASSETS_DIR = intro-game/assets
BIN_DIR = intro-game/bin

# Fichiers sources et objets
SRC_FILES = $(wildcard $(INTRO_SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(INTRO_SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
HEADER_FILES = $(wildcard $(INTRO_INCLUDE_DIR)/*.h)

# Options de compilation
INTRO_CFLAGS = `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Options de débogage
DEBUG_FLAGS = -g

# Nom de l'exécutable
EXECUTABLE = $(BIN_DIR)/introGame

# Variable pour activer/désactiver le mode débogage
INTRO_DEBUG = 0

# Si DEBUG est activé, on ajoute les flags de débogage
ifeq ($(INTRO_DEBUG), 1)
    CFLAGS += $(DEBUG_FLAGS)
endif

# Règle par défaut
intro-game: $(EXECUTABLE)

# Compilation de l'exécutable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@

# Compilation des fichiers objets
$(BIN_DIR)/%.o: $(INTRO_SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)  # Crée le répertoire bin s'il n'existe pas
	$(CC) $(INTRO_CFLAGS) -c $< -o $@

# Nettoyage
intro-clean:
	rm -rf $(BIN_DIR)/*

# Exécution du programme
intro-exe:
	./$(EXECUTABLE)

# Cible pour compiler en mode débogage
intro-debug:
	$(MAKE) INTRO_DEBUG=1

intro-apple : 
	gcc $(INTRO_SRC_DIR)/*.c -o $(BIN_DIR)/introGame -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer -Wl,-rpath,/Library/Frameworks

# ===============================================================
# ====================     Makefile     =========================
# ===============================================================

# all:install tools release

# # Configs

# include mk/dirs.mk

# # Build

# include mk/build.mk
# include mk/tools.mk

# # ===============================================================

# # Utils

# include mk/utils.mk
# include mk/enhanced_term.mk
# include mk/python.mk
# include mk/config.mk
# include mk/docs.mk
# include mk/clean.mk

# # ===============================================================

# # Flags

# include mk/flags.mk

# # ===============================================================

# # Modules

# include mk/modules.mk
# include mk/libs.mk
# include mk/scripts.mk
# include mk/classes.mk

# $(call register_modules, ${MODULES})
# $(call register_modules, ${SCRIPTS_MODULES})
# $(call register_modules, ${CLASSES_MODULES})

# -include $(RELEASE_MODULES:.o=.d)
# -include $(DEBUG_MODULES:.o=.d)

# # ===============================================================

# # Targets

# # === Executables ===

# release: prebuild ${RELEASE_MODULES} ${LIBS}
# 	$(call build_executable, ${BUILD_DIR}/$(RELEASE_DIR), -O3,copy_assets)
# 	$(call launch,${BUILD_DIR}/$(RELEASE_DIR)/$(APP_NAME))

# debug: prebuild ${DEBUG_MODULES} ${LIBS}
# 	$(call build_executable, ${BUILD_DIR}/$(DEBUG_DIR), -g -DDEBUG -O0,copy_assets)

# ${TEST_DIR}/%: prebuild ${TEST_DIR}/%.o ${DEBUG_MODULES} ${LIBS}
# 	$(call build_executable, ${OBJ_DIR}/$(TEST_DIR), , )

# # === Objects ===

# %.o: %.c
# 	$(call build_executable,,, )

# ${OBJ_DIR}/${TEST_DIR}/%.o: %.c
# 	$(call build_object,$(TEST_DIR), -g -DDEBUG)

# # Release objects constructor for .c files
# ${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.c
# 	$(call build_object,$(RELEASE_DIR), -O3)

# # Debug objects constructor for .c files
# ${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.c
# 	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

# # Release objects constructor for .cscript files
# ${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.cscript
# 	$(call build_object,$(RELEASE_DIR), )

# # Debug objects constructor for .cscript files
# ${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.cscript
# 	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

# tools: prebuildtools ${TOOLS}
# 	$(call end_build_tools)

# clear: prebuild
# 	$(call clean)

# clean: prebuild
# 	$(call clean)

# # Default target to install libraries
# install: check-python
# 	$(call install_python_requirements)

# # Ensure that header files are ignored to prevent errors
# %.h:
# 	$(call ignore)

# docs:
# 	$(call generate_docs)

# .PHONY: $(COMMAND_TARGETS)

# # ===============================================================

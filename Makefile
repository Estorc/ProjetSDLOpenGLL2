# Définition du compilateur
CC := gcc

# Détection du système d'exploitation
UNAME_S := $(shell uname -s)

# Dossiers
SRC_DIR := src
INC_DIR := include
BIN_DIR := bin
ASSETS_DIR := assets

# Recherche automatique des fichiers source (.c) dans tous les sous-dossiers
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
HEADER_FILES := $(shell find $(INC_DIR) -type f -name '*.h')

# Options de compilation pour Linux et macOS
CFLAGS := -I$(INC_DIR) `sdl2-config --cflags`
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Gestion spécifique pour macOS
ifeq ($(UNAME_S), Darwin)
    CFLAGS := -I$(INC_DIR) -F/Library/Frameworks
    LDFLAGS := -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer -Wl,-rpath,/Library/Frameworks
endif

# Debug Mode
DEBUG_FLAGS := -g
ifeq ($(DEBUG), 1)
    CFLAGS += $(DEBUG_FLAGS)
endif

# Nom de l'exécutable
EXECUTABLE := $(BIN_DIR)/introGame

# Règle par défaut
all: $(EXECUTABLE)

# Compilation de l'exécutable
$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@

# Compilation des fichiers objets
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BIN_DIR)/*

# Exécution
run: all
	./$(EXECUTABLE)

# Compilation en mode Debug
debug:
	$(MAKE) DEBUG=1

.PHONY: all clean run debug
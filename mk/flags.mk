MD += -MD

LFLAGS += `sdl2-config --cflags --libs`
LFLAGS += -lswscale -lavcodec -lavformat -lavutil
LFLAGS += -lm
LFLAGS += -Isrc
LFLAGS += -Iinclude
LFLAGS += -Ilib
LFLAGS += -lSDL2_image
LFLAGS += -lSDL2_ttf
LFLAGS += -lSDL2_mixer
LFLAGS += -lCUnit
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
WFLAGS += -Wno-unknown-pragmas
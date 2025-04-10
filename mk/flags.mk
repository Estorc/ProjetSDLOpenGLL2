MD += -MD

LFLAGS += `sdl2-config --cflags --libs`
LFLAGS += -lSDL2_mixer
LFLAGS += -lSDL2_image
LFLAGS += -lSDL2_ttf
LFLAGS += -lm
LFLAGS += -Isrc
LFLAGS += -Iinclude
LFLAGS += -Llib
LFLAGS += -Llib/linux
LFLAGS += -lavformat -lavcodec -lavutil -lswscale -lswresample -lz
LFLAGS += -lz -llzma
LFLAGS += -lcunit
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
LFLAGS += -ldrm
endif

WFLAGS += -Wall
WFLAGS += -Wno-implicit-function-declaration
WFLAGS += -Wno-unknown-pragmas
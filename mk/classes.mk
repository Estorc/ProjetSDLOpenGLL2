ifeq ($(OS),Windows_NT) 
CLASS_TOOLS := $(shell which tools/class_tools.exe)
else
CLASS_TOOLS := $(shell which tools/class_tools)
endif

ifeq ($(CLASS_TOOLS),)
NEED_BUILD_TOOLS := 1
else
CLASS_TOOLS := $(shell $(CLASS_TOOLS))
endif

CLASSES_MODULES := $(wildcard $(PROCESSED_CLASS_DIR)/*.class.c)
CLASSES_MODULES := $(patsubst %.class.c, %.class.o, $(CLASSES_MODULES))
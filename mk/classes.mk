ifeq ($(OS),Windows_NT) 
CLASS_TOOLS := $(shell tools/class_tools.exe)
else
CLASS_TOOLS := $(shell tools/class_tools)
endif
CLASSES_MODULES := $(wildcard $(PROCESSED_CLASS_DIR)/*.class.c)
CLASSES_MODULES := $(patsubst %.class.c, %.class.o, $(CLASSES_MODULES))
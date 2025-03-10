SCRIPTS_PATHS := $(shell find src/scripts -type f -name "*.cscript")
SCRIPTS_MODULES := $(SCRIPTS_PATHS:.cscript=.o)
# ===============================================================
# ====================     Makefile     =========================
# ===============================================================

all:install tools release

# Configs

include mk/dirs.mk

# Build

include mk/build.mk
include mk/tools.mk

# ===============================================================

# Utils

include mk/utils.mk
include mk/enhanced_term.mk
include mk/python.mk
include mk/config.mk
include mk/docs.mk
include mk/clean.mk

# ===============================================================

# Flags

include mk/flags.mk

# ===============================================================

# Modules

include mk/modules.mk
include mk/libs.mk
include mk/scripts.mk
include mk/classes.mk

$(call register_modules, ${MODULES})
$(call register_modules, ${SCRIPTS_MODULES})
$(call register_modules, ${CLASSES_MODULES})

-include $(RELEASE_MODULES:.o=.d)
-include $(DEBUG_MODULES:.o=.d)

# ===============================================================

# Targets

# === Executables ===

release: prebuild ${RELEASE_MODULES} ${LIBS}
	$(call build_executable, ${BUILD_DIR}/$(RELEASE_DIR), -O3,copy_assets)
	$(call launch,${BUILD_DIR}/$(RELEASE_DIR)/$(APP_NAME))

debug: prebuild ${DEBUG_MODULES} ${LIBS}
	$(call build_executable, ${BUILD_DIR}/$(DEBUG_DIR), -g -DDEBUG -O0,copy_assets)

${TEST_DIR}/%: prebuild ${TEST_DIR}/%.o ${DEBUG_MODULES} ${LIBS}
	$(call build_executable, ${OBJ_DIR}/$(TEST_DIR), , )

# === Objects ===

%.o: %.c
	$(call build_executable,,, )

${OBJ_DIR}/${TEST_DIR}/%.o: %.c
	$(call build_object,$(TEST_DIR), -g -DDEBUG)

# Release objects constructor for .c files
${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.c
	$(call build_object,$(RELEASE_DIR), -O3)

# Debug objects constructor for .c files
${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.c
	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

# Release objects constructor for .cscript files
${OBJ_DIR}/$(RELEASE_DIR)/%.o: %.cscript
	$(call build_object,$(RELEASE_DIR), )

# Debug objects constructor for .cscript files
${OBJ_DIR}/$(DEBUG_DIR)/%.o: %.cscript
	$(call build_object,$(DEBUG_DIR), -g -DDEBUG)

tools: prebuildtools ${TOOLS}
	$(call end_build_tools)

clear: prebuild
	$(call clean)

clean: prebuild
	$(call clean)

# Default target to install libraries
install: check-python
	$(call install_python_requirements)

# Ensure that header files are ignored to prevent errors
%.h:
	$(call ignore)

docs:
	$(call generate_docs)

.PHONY: $(COMMAND_TARGETS)

# ===============================================================
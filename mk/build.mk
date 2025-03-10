GCC = gcc

prebuild:
	@clear
	@$(call PRINT_SEPARATOR,Begin build.)

define register_modules
	$(eval RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/,${1}))
	$(eval DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/,${1}))
endef

define launch
	@$(call PRINT_SEPARATOR,Launch ${1}...)
	@${1}
endef

define failed_build
	$(call PRINT_SEPARATOR,${ERROR_COL}Build failed!) \
	exit 1
endef

define build_executable
	@$(call check_tools)

	@mkdir -p ${1}
	@$(call PRINT_CENTERED,${ACT_COL}Linking ${FILE_COL}\"${APP_NAME}\"${NC}...)
	@${GCC} -o ${1}/${APP_NAME} $(call requirements) ${LFLAGS} ${WFLAGS} ${2}

	@if [ ${3} = "copy_assets" ]; then \
		$(call PRINT_CENTERED,${ACT_COL}Copying assets...); \
		rsync -rupE assets ${1}/; \
		$(call PRINT_CENTERED,${SUCCESS_COL}Assets copied!); \
		$(call PRINT_CENTERED,${ACT_COL}Copying shaders...); \
		rsync -rupE shaders ${1}/; \
		$(call PRINT_CENTERED,${SUCCESS_COL}Shaders copied!); \
	fi

	@$(call PRINT_SEPARATOR,${SUCCESS_COL}Build successful!)
endef

# Function to handle common preprocessing and compilation
define build_object
	@$(call check_tools)

	@$(call PRINT_CENTERED,${ACT_COL}Preprocessing ${FILE_COL}\"$<\"${NC} => ${FILE_COL}\"$<~\"${NC}...);
	@$(PYTHON) ./tools/preprocessor_pipeline.py $< $<~

	@$(call PRINT_CENTERED,${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...);
	@mkdir -p ${OBJ_DIR}/${1}/${dir $*}
	@${GCC} -x c -c $<~ -o ${OBJ_DIR}/${1}/$*.o -I$(dir $*) ${MD} ${DFLAGS} ${LFLAGS} ${WFLAGS} ${2}
	@sed -i 's#$<~#$<#g' ${OBJ_DIR}/${1}/$*.d
	@$(call PRINT_CENTERED,${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${OBJ_DIR}/${1}/$*.o);

	@if [ ${KEEP_TEMP_FILES} = "0" ]; then \
		rm -rf $<~; \
	fi
endef


define ignore
	@$(call PRINT_CENTERED,${ACT_COL}Ignoring $@);
endef
GCC = gcc

prebuild:
	@printf "${STEP_COL}===================== Begin build. ====================${NC}\n"

define register_modules
	$(eval RELEASE_MODULES := $(RELEASE_MODULES) $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/,${1}))
	$(eval DEBUG_MODULES := $(DEBUG_MODULES) $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/,${1}))
endef

define launch
	@printf "${STEP_COL}=================== Launch ${1}... =================${NC}\n"
	@${1}
endef

define build_executable
	@mkdir -p ${1}
	@printf "${ACT_COL}Linking ${FILE_COL}\"${APP_NAME}\"${NC}...\n"
	@${GCC} -o ${1}/${APP_NAME} $(call requirements) ${LFLAGS} ${WFLAGS} ${2}

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
	@printf "${ACT_COL}Preprocessing ${FILE_COL}\"$<\"${NC} => ${FILE_COL}\"$<~\"${NC}...\n"
	@$(PYTHON) ./tools/preprocessor_pipeline.py $< $<~

	@printf "${ACT_COL}Building ${FILE_COL}\"$*\"${NC}...\n"
	@mkdir -p ${OBJ_DIR}/${1}/${dir $*}
	@${GCC} -x c -c $<~ -o ${OBJ_DIR}/${1}/$*.o -I$(dir $*) ${MD} ${DFLAGS} ${LFLAGS} ${WFLAGS} ${2}
	@sed -i 's#$<~#$<#g' ${OBJ_DIR}/${1}/$*.d
	@printf "${SUCCESS_COL}Builded ${FILE_COL}\"$*\"${NC} => ${SUCCESS_COL}${OBJ_DIR}/${1}/$*.o${NC}\n"

	@if [ ${KEEP_TEMP_FILES} = "0" ]; then \
		rm -rf $<~; \
	fi
endef


define ignore
	@printf "${ACT_COL}Ignoring $@${NC}\n"
endef
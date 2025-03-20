# Tools enumeration

NEED_BUILD_TOOLS := 0
TOOLS += ${TOOLS_DIR}/class_tools
TOOLS += ${TOOLS_DIR}/preprocessor_pipeline

# Targets

prebuildtools:
	@clear
	@$(call PRINT_SEPARATOR,Begin build tools.)

${TOOLS_DIR}/%: ${TOOLS_DIR}/%.c
	@$(call PRINT_CENTERED,${ACT_COL}Build ${notdir $@}...)
	@${GCC} -o $@ $^ ${DFLAGS} ${LFLAGS} ${WFLAGS} -g -Wno-format

define end_build_tools
	@$(call PRINT_SEPARATOR,${SUCCESS_COL}Successfully build the tools!)
endef

define check_tools
	@if [ $(NEED_BUILD_TOOLS) = "1" ]; then \
		$(call PRINT_CENTERED,Tools not up-to-date!); \
		$(call PRINT_CENTERED,Run ${FILE_COL}make tools${NC} to build tools); \
		$(call failed_build); \
	fi
endef
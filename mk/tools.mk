# Tools enumeration

TOOLS += ${TOOLS_DIR}/class_tools

# Targets

prebuildtools:
	@printf "${STEP_COL}===================== Begin build tools. ====================${NC}\n"

${TOOLS_DIR}/%: ${TOOLS_DIR}/%.c
	@printf "${ACT_COL}Build ${notdir $@}...${NC}\n"
	@${GCC} -o $@ $^ ${DFLAGS} ${WFLAGS} -Wno-format

define end_build_tools
	@printf "${STEP_COL}============= ${SUCCESS_COL}Successfully build the tools!${STEP_COL} =============${NC}\n"
endef
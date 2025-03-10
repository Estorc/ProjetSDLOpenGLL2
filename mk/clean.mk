define clean
	@printf "${ACT_COL}Clear the build...${NC}\n"
	@rm -rf ${PROCESSED_CLASS_DIR}
	@rm -rf ${BUILD_DIR}
	@rm -rf ${OBJ_DIR}
	@rm -rf ${TOOLS_DIR}/*.exe
	@rm -rf ${TOOLS_DIR}/*.a
	@find . -type f -name "*.c~" -exec rm -f {} \;
	@printf "${SUCCESS_COL}Successfully clear the build!${NC}\n"
endef
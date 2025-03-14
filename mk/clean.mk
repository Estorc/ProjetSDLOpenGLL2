define clean
	@$(call PRINT_CENTERED,${ACT_COL}Clear the build...)
	@rm -rf ${PROCESSED_CLASS_DIR}
	@rm -rf ${BUILD_DIR}
	@rm -rf ${OBJ_DIR}
	@rm -rf ${TOOLS_DIR}/*.exe
	@rm -rf ${TOOLS_DIR}/*.a
	@find . -type f -name "*.c~" -exec rm -f {} \;
	@$(call PRINT_SEPARATOR,${SUCCESS_COL}Successfully clear the build!)
endef
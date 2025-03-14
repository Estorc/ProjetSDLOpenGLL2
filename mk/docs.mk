define generate_docs
	@$(call PRINT_CENTERED,${ACT_COL}Generate documentation...)
	@doxygen Doxyfile
	@$(call PRINT_CENTERED,${SUCCESS_COL}Documentation generated!)
endef
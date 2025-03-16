define generate_docs
	@$(call PRINT_CENTERED,${ACT_COL}Generate documentation...)
	@doxygen Doxyfile
	@$(call PRINT_CENTERED,${SUCCESS_COL}Documentation generated!)
	@$(call PRINT_CENTERED,${ACT_COL}Postprocessing documentation...)
	@$(PYTHON) ./docs/dots_postprocessing.py
	@$(call PRINT_CENTERED,${SUCCESS_COL}Documentation builded!)
endef
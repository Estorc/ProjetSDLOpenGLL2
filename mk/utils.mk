define requirements
	$(filter-out $(COMMAND_TARGETS),$^)
endef
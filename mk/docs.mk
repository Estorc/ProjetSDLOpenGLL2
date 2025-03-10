define generate_docs
	@printf "${ACT_COL}Generate documentation...${NC}\n"
	@doxygen Doxyfile
	@printf "${SUCCESS_COL}Documentation generated!${NC}\n"
endef
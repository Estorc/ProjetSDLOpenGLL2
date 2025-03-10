PYTHON = python3
PIP = $(shell which pip)
ifeq ($(PIP),)
	HAVE_PIP = 0
else
	HAVE_PIP = 1
endif
PYTHON_REQUIREMENTS = ./lib/python_requirements.txt


define install_python_requirements
	@printf "${ACT_COL}Install python requirements...${NC}\n"
	@$(PIP) install -r $(PYTHON_REQUIREMENTS)
	@printf "${SUCCESS_COL}Successfully install python requirements!${NC}\n"
endef

# Ensure that the environment is not externally-managed
check-python:
	@if [ $(HAVE_PIP) = "0" ]; then \
		echo "Error: pip is not installed. Please install pip."; \
		exit 1; \
	elif [ -f "/usr/lib/python$(shell $(PYTHON) -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')" ]; then \
		echo "Warning: You're using a system-managed Python environment. Installing packages globally may cause conflicts."; \
		echo "Consider using a virtual environment for your project."; \
		exit 1; \
	fi
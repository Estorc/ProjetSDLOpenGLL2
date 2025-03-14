PYTHON = python3
PIP = $(shell which pip)
ifeq ($(PIP),)
	HAVE_PIP = 0
else
	HAVE_PIP = 1
endif
PYTHON_REQUIREMENTS = ./lib/python_requirements.txt


define install_python_requirements
	@$(call PRINT_CENTERED,${ACT_COL}Install python requirements...)
	@$(PIP) install -r $(PYTHON_REQUIREMENTS)
	@$(call PRINT_CENTERED,${SUCCESS_COL}Successfully install python requirements!)
endef

# Ensure that the environment is not externally-managed
check-python:
	@if [ $(HAVE_PIP) = "0" ]; then \
		$(call PRINT_CENTERED,Error: pip is not installed. Please install pip.); \
		$(call failed_build); \
	elif [ -f "/usr/lib/python$(shell $(PYTHON) -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')" ]; then \
		echo "Warning: You're using a system-managed Python environment. Installing packages globally may cause conflicts."; \
		echo "Consider using a virtual environment for your project."; \
		$(call failed_build); \
	fi
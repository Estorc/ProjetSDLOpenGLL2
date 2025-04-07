TESTS := $(wildcard $(TEST_DIR)/*.c)
TESTS := $(addprefix $(OBJ_DIR)/,${TESTS})
TESTS := $(TESTS:.c=.o)

GLOBAL_TEST := test_global

CALL_TESTS = $(foreach TEST, $(filter-out $(GLOBAL_TEST),$(notdir $(basename $(TESTS)))), int test_$(TEST)_bis();)
CALL_TESTS += $(foreach TEST, $(filter-out $(GLOBAL_TEST),$(notdir $(basename $(TESTS)))), test_$(TEST)_bis();)
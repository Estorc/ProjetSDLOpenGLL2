#include <unistd.h>
#include <stdio.h>
#include <io/osio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#ifndef CALL_TESTS
#define CALL_TESTS printf("No tests to run.\n")
#endif

int filter(const dirent_t *entry) {
    return (entry->d_type != DT_DIR && entry->d_name[0] != '.' && entry->d_name[0] != '_');
}

int main(int argc, char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CALL_TESTS;

    CU_basic_set_mode(CU_BRM_VERBOSE);
    if (CUE_SUCCESS != CU_basic_run_tests()) {
        printf("Tests failed\n");
        return -1;
    }

    CU_cleanup_registry();
    return 0;
}
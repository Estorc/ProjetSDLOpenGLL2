#pragma once

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#ifndef TEST_NAME
#define TEST_NAME test
#endif
#define test TEST_NAME

#define CONCAT(a, b) a ## b
#define EXPAND_AND_CONCAT(a, b) CONCAT(a, b)

#ifdef GLOBAL_MAIN
#undef main
#define main EXPAND_AND_CONCAT(__unused_main_, TEST_NAME)
#endif
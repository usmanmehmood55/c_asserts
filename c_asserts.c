/**
 * @file     c_asserts.c
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    A rudimentary library for asserts
 * @version  0.1
 * @date     2023-05-01
 * 
 * Copyright (c) 2023, Usman Mehmood
 */

#include "c_asserts.h"

test_environment_t test_env = 
{
    .failed_tests = 0,
    .passed_tests = 0,
    .total_tests  = 0
};

int display_test_summary(void) 
{
    printf("\n");
    printf("----- Test Summary -----\n");
    printf(" Total tests:  \033[1;36m%02d\033[0m\n", test_env.total_tests);
    printf(" Passed tests: \033[1;32m%02d\033[0m\n", test_env.passed_tests);
    printf(" Failed tests: \033[1;31m%02d\033[0m\n", test_env.failed_tests);
    printf("------------------------\n");

    return (test_env.total_tests == test_env.passed_tests) ? 0 : -1;
}

// end of c_asserts.c
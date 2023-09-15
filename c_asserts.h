/**
 * @file     c_asserts.h
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    A rudimentary library for asserts
 * @version  0.1
 * @date     2023-05-01
 * 
 * Copyright (c) 2023, Usman Mehmood
 */

#ifndef C_ASSERTS_H_
#define C_ASSERTS_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct test_environment_t
{
    int passed_tests;
    int failed_tests;
    int total_tests;
} test_environment_t;

extern test_environment_t test_env;

#define PATH(file) (strrchr((file), '/') ? (strrchr((file), '/') + 1) : (file))

#define ASSERT_TEST(condition)                                                                              \
    do                                                                                                      \
    {                                                                                                       \
        if (!(condition))                                                                                   \
        {                                                                                                   \
            printf("\033[1;31mAssertion failed at %s:%d:%s\033[0m\n", PATH(__FILE__), __LINE__, __func__);  \
            return false;                                                                                   \
        }                                                                                                   \
    } while (0)

#define TEST(func)                                                                                             \
    do                                                                                                         \
    {                                                                                                          \
        test_env.total_tests++;                                                                                \
        if (func())                                                                                            \
        {                                                                                                      \
            printf("\033[1;32m[PASSED]\033[0m %s:%d \033[1;32m%s\033[0m\n", PATH(__FILE__), __LINE__, #func);  \
            test_env.passed_tests++;                                                                           \
        }                                                                                                      \
        else                                                                                                   \
        {                                                                                                      \
            printf("\033[1;31m[FAILED]\033[0m %s:%d \033[1;31m%s\033[0m\n", PATH(__FILE__), __LINE__, #func);  \
            test_env.failed_tests++;                                                                           \
        }                                                                                                      \
    } while (0)

int display_test_summary(void);

#endif // C_ASSERTS_H_
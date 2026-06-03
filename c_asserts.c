/**
 * @file     c_asserts.c
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    A C assertion and unit test library
 * @version  2.0
 * @date     01-05-2023
 *
 * Copyright (c) 2023, Usman Mehmood
 */

#include "c_asserts.h"

#include <inttypes.h>
#include <stdarg.h>

#define C_ASSERT_COLOR_RED "\033[1;31m"
#define C_ASSERT_COLOR_GREEN "\033[1;32m"
#define C_ASSERT_COLOR_YELLOW "\033[1;33m"
#define C_ASSERT_COLOR_CYAN "\033[1;36m"
#define C_ASSERT_COLOR_CLEAR "\033[0m"
#define C_ASSERT_MEMORY_DUMP_LIMIT 256U

/**
 * @brief Tracks whether failure output for the current test has started.
 *
 * This keeps multi-assert failures grouped together while preserving a blank
 * line before the next failed test block.
 */
static bool _c_assert_failure_block_open = false;

/**
 * @brief Prints a formatted failure message in the current failure block.
 *
 * @param[in] format `printf`-style format string.
 * @param[in] ...    Format arguments.
 */
static void _c_assert_print_failure(const char *format, ...)
{
    va_list args;

    if (!_c_assert_failure_block_open)
    {
        printf("\n");
        _c_assert_failure_block_open = true;
    }

    printf(C_ASSERT_COLOR_RED);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(C_ASSERT_COLOR_CLEAR "\n");
}

test_environment_t test_env =
{
    .failed_tests          = 0,
    .passed_tests          = 0,
    .not_implemented_tests = 0,
    .total_tests           = 0
};

/**
 * @brief Starts a unit test suite and clears previous result counters.
 */
void _ut_start(void)
{
    test_env.failed_tests          = 0;
    test_env.passed_tests          = 0;
    test_env.not_implemented_tests = 0;
    test_env.total_tests           = 0;
    _c_assert_failure_block_open   = false;
}

/**
 * @brief Converts a path into a display-friendly file name.
 */
const char *_c_assert_path(const char *file)
{
    const char *last_forward_slash = NULL;
    const char *last_back_slash = NULL;

    if (file == NULL)
    {
        return "(null)";
    }

    last_forward_slash = strrchr(file, '/');
    last_back_slash = strrchr(file, '\\');

    if ((last_forward_slash == NULL) && (last_back_slash == NULL))
    {
        return file;
    }

    if (last_forward_slash == NULL)
    {
        return last_back_slash + 1;
    }

    if (last_back_slash == NULL)
    {
        return last_forward_slash + 1;
    }

    if (last_forward_slash > last_back_slash)
    {
        return last_forward_slash + 1;
    }

    return last_back_slash + 1;
}

/**
 * @brief Checks a generic boolean condition and reports it when false.
 */
bool _c_assert_check(const char *func_name, const char *file, int line, bool condition, const char *expression)
{
    if (!condition)
    {
        _c_assert_print_failure("Assertion failed at %s:%d:%s: %s", PATH(file), line, func_name, expression);
    }

    return condition;
}

/**
 * @brief Checks that a subtest result is `UT_PASSED`.
 */
bool _c_assert_subtest(const char *func_name, const char *file, int line, unit_test_result_t subtest,
                      const char *subtest_name)
{
    bool passing = (subtest == UT_PASSED);

    if (!passing)
    {
        _c_assert_print_failure("Subtest failed at %s:%d:%s: %s", PATH(file), line, func_name, subtest_name);
    }

    return passing;
}

/**
 * @brief Checks that a boolean value is true.
 */
bool _c_assert_true(const char *func_name, const char *file, int line, bool value, const char *value_name)
{
    bool passing = (value == true);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s to be true, got false", PATH(file), line, func_name,
                               value_name);
    }

    return passing;
}

/**
 * @brief Checks that a boolean value is false.
 */
bool _c_assert_false(const char *func_name, const char *file, int line, bool value, const char *value_name)
{
    bool passing = (value == false);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s to be false, got true", PATH(file), line, func_name,
                               value_name);
    }

    return passing;
}

/**
 * @brief Checks that a signed integer equals an expected value.
 */
bool _c_assert_int(const char *func_name, const char *file, int line, intmax_t value, intmax_t expected,
                   const char *value_name, const char *expected_name)
{
    bool passing = (value == expected);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s == %s, got %" PRIdMAX ", expected %" PRIdMAX, PATH(file), line,
                                func_name, value_name, expected_name, value, expected);
    }

    return passing;
}

/**
 * @brief Checks that an unsigned integer equals an expected value.
 */
bool _c_assert_uint(const char *func_name, const char *file, int line, uintmax_t value, uintmax_t expected,
                    const char *value_name, const char *expected_name)
{
    bool passing = (value == expected);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s == %s, got %" PRIuMAX " (0x%" PRIXMAX "), expected %" PRIuMAX
                               " (0x%" PRIXMAX ")",
                                PATH(file), line, func_name, value_name, expected_name, value, value, expected,
                                expected);
    }

    return passing;
}

/**
 * @brief Checks that a 64-bit unsigned integer equals an expected value.
 */
bool _c_assert_uint64(const char *func_name, const char *file, int line, uint64_t value, uint64_t expected,
                     const char *value_name, const char *expected_name)
{
    bool passing = (value == expected);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s == %s, got %" PRIu64 " (0x%" PRIX64 "), expected %" PRIu64
                               " (0x%" PRIX64 ")",
                               PATH(file), line, func_name, value_name, expected_name, value, value, expected,
                               expected);
    }

    return passing;
}

/**
 * @brief Checks that an unsigned integer is inside an expected range.
 */
bool _c_assert_uint_range(const char *func_name, const char *file, int line, uintmax_t value, uintmax_t expected,
                          uintmax_t margin, const char *value_name, const char *expected_name)
{
    uintmax_t min = (expected > margin) ? (expected - margin) : 0U;
    uintmax_t max = ((UINTMAX_MAX - expected) > margin) ? (expected + margin) : UINTMAX_MAX;
    bool passing = ((min <= value) && (value <= max));

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s near %s, got %" PRIuMAX " (0x%" PRIXMAX "), expected %" PRIuMAX
                               " +/- %" PRIuMAX " [%" PRIuMAX ", %" PRIuMAX "]",
                                PATH(file), line, func_name, value_name, expected_name, value, value, expected,
                                margin, min, max);
    }

    return passing;
}

/**
 * @brief Checks that a pointer equals an expected pointer.
 */
bool _c_assert_ptr(const char *func_name, const char *file, int line, const void *value, const void *expected,
                  const char *value_name, const char *expected_name)
{
    bool passing = (value == expected);

    if (!passing)
    {
        _c_assert_print_failure("%s:%d:%s expected %s == %s, got %p, expected %p", PATH(file), line, func_name,
                               value_name, expected_name, (void *)value, (void *)expected);
    }

    return passing;
}

/**
 * @brief Checks two memory regions and prints a mismatch table on failure.
 */
bool _c_assert_memory(const char *func_name, const char *file, int line, const void *buffer,
                     const void *expected_buffer, size_t size, const char *buffer_name,
                     const char *expected_buffer_name)
{
    const unsigned char *actual = (const unsigned char *)buffer;
    const unsigned char *expected = (const unsigned char *)expected_buffer;
    size_t first_difference = 0U;
    size_t limit = (size > C_ASSERT_MEMORY_DUMP_LIMIT) ? C_ASSERT_MEMORY_DUMP_LIMIT : size;

    if ((size == 0U) || (buffer == expected_buffer))
    {
        return true;
    }

    if ((buffer == NULL) || (expected_buffer == NULL))
    {
        _c_assert_print_failure("%s:%d:%s expected memory %s == %s, got %p, expected %p", PATH(file), line,
                               func_name, buffer_name, expected_buffer_name, (void *)buffer,
                               (void *)expected_buffer);
        return false;
    }

    if (memcmp(buffer, expected_buffer, size) == 0)
    {
        return true;
    }

    while ((first_difference < size) && (actual[first_difference] == expected[first_difference]))
    {
        first_difference++;
    }

    if (!_c_assert_failure_block_open)
    {
        printf("\n");
        _c_assert_failure_block_open = true;
    }

    printf(C_ASSERT_COLOR_RED "%s:%d:%s memory mismatch for %s and %s (%zu bytes)" C_ASSERT_COLOR_CLEAR "\n",
           PATH(file), line, func_name, buffer_name, expected_buffer_name, size);
    printf("First difference at offset 0x%zX: expected 0x%02X, got 0x%02X\n", first_difference, (unsigned int)expected[first_difference], (unsigned int)actual[first_difference]);
    printf("Offset | Expected | Actual | Diff\n");
    printf("-------+----------+--------+-----\n");

    for (size_t i = 0U; i < limit; i++)
    {
        char mark = (actual[i] == expected[i]) ? ' ' : '!';
        printf("0x%04zX | 0x%02X     | 0x%02X   | %c\n", i, (unsigned int)expected[i], (unsigned int)actual[i], mark);
    }

    if (limit < size)
    {
        printf("... truncated after %u bytes\n", (unsigned int)C_ASSERT_MEMORY_DUMP_LIMIT);
    }

    return false;
}

/**
 * @brief Reads a bit from a 32-bit value.
 */
uint32_t _c_assert_read_bit(uint32_t register_value, uint8_t bit_position)
{
    if (bit_position >= 32U)
    {
        return 0U;
    }

    return (register_value >> bit_position) & 1U;
}

/**
 * @brief Reads a bit range from a 32-bit value.
 */
uint32_t _c_assert_read_bit_range(uint32_t register_value, uint8_t start_position, uint8_t length)
{
    uint32_t mask = 0U;

    if ((start_position >= 32U) || (length == 0U))
    {
        return 0U;
    }

    if (length >= (32U - start_position))
    {
        mask = UINT32_MAX << start_position;
    }
    else
    {
        mask = ((1UL << length) - 1U) << start_position;
    }

    return (register_value & mask) >> start_position;
}

/**
 * @brief Records and prints one test result.
 */
void _c_assert_run_test(unit_test_result_t result, const char *file, int line, const char *test_name)
{
    test_env.total_tests++;

    switch (result)
    {
    case UT_PASSED:
        printf(C_ASSERT_COLOR_GREEN "[PASSED]" C_ASSERT_COLOR_CLEAR " %s:%d " C_ASSERT_COLOR_GREEN "%s" C_ASSERT_COLOR_CLEAR "\n", PATH(file), line, test_name);
        test_env.passed_tests++;
        break;

    case UT_NOT_IMPLEMENTED:
        printf(C_ASSERT_COLOR_YELLOW "[NO IMP]" C_ASSERT_COLOR_CLEAR " %s:%d " C_ASSERT_COLOR_YELLOW "%s" C_ASSERT_COLOR_CLEAR "\n", PATH(file), line, test_name);
        test_env.not_implemented_tests++;
        break;

    case UT_FAILED:
        printf(C_ASSERT_COLOR_RED "[FAILED]" C_ASSERT_COLOR_CLEAR " %s:%d " C_ASSERT_COLOR_RED "%s"
               C_ASSERT_COLOR_CLEAR "\n",
               PATH(file), line, test_name);
        test_env.failed_tests++;
        _c_assert_failure_block_open = false;
        break;

    default:
        _c_assert_print_failure("Illegal test result from %s: 0x%X", test_name, result);
        printf(C_ASSERT_COLOR_RED "[FAILED]" C_ASSERT_COLOR_CLEAR " %s:%d " C_ASSERT_COLOR_RED "%s"
               C_ASSERT_COLOR_CLEAR "\n",
               PATH(file), line, test_name);
        test_env.failed_tests++;
        _c_assert_failure_block_open = false;
        break;
    }
}

/**
 * @brief Prints the final test summary.
 */
int _display_test_summary(void)
{
    int pass_percentage = 0;

    if (test_env.total_tests > 0)
    {
        pass_percentage = (test_env.passed_tests * 100) / test_env.total_tests;
    }

    printf("\n");
    printf("----- Test Summary -----\n");
    printf(" Total tests:       " C_ASSERT_COLOR_CYAN "%02d" C_ASSERT_COLOR_CLEAR "\n", test_env.total_tests);
    printf(" Passed tests:      " C_ASSERT_COLOR_GREEN "%02d" C_ASSERT_COLOR_CLEAR "\n", test_env.passed_tests);
    printf(" Failed tests:      " C_ASSERT_COLOR_RED "%02d" C_ASSERT_COLOR_CLEAR "\n", test_env.failed_tests);
    printf(" Not implemented:   " C_ASSERT_COLOR_YELLOW "%02d" C_ASSERT_COLOR_CLEAR "\n", test_env.not_implemented_tests);
    printf(" Pass percentage:   " C_ASSERT_COLOR_GREEN "%02d%%" C_ASSERT_COLOR_CLEAR "\n", pass_percentage);
    printf("------------------------\n");

    return (test_env.total_tests == test_env.passed_tests) ? 0 : -1;
}

// end of c_asserts.c

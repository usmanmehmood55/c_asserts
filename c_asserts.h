/**
 * @file     c_asserts.h
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    A C assertion and unit test library
 * @version  2.0
 * @date     01-05-2023
 *
 * Copyright (c) 2023, Usman Mehmood
 */

#ifndef C_ASSERTS_H_
#define C_ASSERTS_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Holds aggregate test counts for the current test run.
 *
 * @warning This variable is managed by the assertion library and should be
 * treated as read-only by tests.
 */
typedef struct test_environment_t
{
    int passed_tests;          ///< Number of tests that returned `UT_PASSED`.
    int failed_tests;          ///< Number of tests that returned `UT_FAILED`.
    int not_implemented_tests; ///< Number of tests that returned `UT_NOT_IMPLEMENTED`.
    int total_tests;           ///< Number of tests executed by `TEST()`.
} test_environment_t;

/**
 * @brief Result returned by every test function.
 */
typedef enum unit_test_result_t
{
    UT_FAILED = 0x0BAD, ///< Test failed.
    UT_PASSED,          ///< Test passed.
    UT_NOT_IMPLEMENTED  ///< Test is intentionally not implemented yet.
} unit_test_result_t;

/**
 * @brief Global test environment used to track test results.
 */
extern test_environment_t test_env;

/**
 * @brief Returns the file name portion of a path.
 *
 * @param[in] file Path to shorten.
 */
#define PATH(file) _c_assert_path(file)

/**
 * @brief Asserts that a boolean value is true.
 *
 * On failure, sets `_ut_result` to `UT_FAILED` and jumps to the current test's
 * `cleanup` label. On success, sets `_ut_result` to `UT_PASSED`.
 *
 * @param[in] value Boolean expression or value to check.
 */
#define ASSERT_TRUE(value)                                                  \
    do                                                                      \
    {                                                                       \
        if (!_c_assert_true(__func__, __FILE__, __LINE__, (value), #value)) \
        {                                                                   \
            _ut_result = UT_FAILED;                                         \
            goto cleanup;                                                   \
        }                                                                   \
        _ut_result = UT_PASSED;                                             \
    } while (0)

/**
 * @brief Asserts that a boolean value is false.
 *
 * @param[in] value Boolean expression or value to check.
 */
#define ASSERT_FALSE(value)                                                  \
    do                                                                       \
    {                                                                        \
        if (!_c_assert_false(__func__, __FILE__, __LINE__, (value), #value)) \
        {                                                                    \
            _ut_result = UT_FAILED;                                          \
            goto cleanup;                                                    \
        }                                                                    \
        _ut_result = UT_PASSED;                                              \
    } while (0)

/**
 * @brief Asserts that a signed integer equals an expected signed integer.
 *
 * @param[in] value          Signed integer value to check.
 * @param[in] expected_value Expected signed integer value.
 */
#define ASSERT_INT(value, expected_value)                                                                       \
    do                                                                                                          \
    {                                                                                                           \
        if (!_c_assert_int(__func__, __FILE__, __LINE__, (intmax_t)(value), (intmax_t)(expected_value), #value, \
                           #expected_value))                                                                    \
        {                                                                                                       \
            _ut_result = UT_FAILED;                                                                             \
            goto cleanup;                                                                                       \
        }                                                                                                       \
        _ut_result = UT_PASSED;                                                                                 \
    } while (0)

/**
 * @brief Asserts that an unsigned integer equals an expected unsigned integer.
 *
 * @param[in] value          Unsigned integer value to check.
 * @param[in] expected_value Expected unsigned integer value.
 */
#define ASSERT_UINT(value, expected_value)                                                                         \
    do                                                                                                             \
    {                                                                                                              \
        if (!_c_assert_uint(__func__, __FILE__, __LINE__, (uintmax_t)(value), (uintmax_t)(expected_value), #value, \
                            #expected_value))                                                                      \
        {                                                                                                          \
            _ut_result = UT_FAILED;                                                                                \
            goto cleanup;                                                                                          \
        }                                                                                                          \
        _ut_result = UT_PASSED;                                                                                    \
    } while (0)

/**
 * @brief Asserts that a 64-bit unsigned integer equals an expected value.
 *
 * @param[in] value          64-bit unsigned integer value to check.
 * @param[in] expected_value Expected 64-bit unsigned integer value.
 */
#define ASSERT_UINT64(value, expected_value)                                                               \
    do                                                                                                     \
    {                                                                                                      \
        if (!_c_assert_uint64(__func__, __FILE__, __LINE__, (uint64_t)(value), (uint64_t)(expected_value), \
                              #value, #expected_value))                                                    \
        {                                                                                                  \
            _ut_result = UT_FAILED;                                                                        \
            goto cleanup;                                                                                  \
        }                                                                                                  \
        _ut_result = UT_PASSED;                                                                            \
    } while (0)

/**
 * @brief Asserts that an unsigned integer is within an expected range.
 *
 * The accepted range is `expected_value +/- margin`, clamped to the valid
 * `uintmax_t` range.
 *
 * @param[in] value          Unsigned integer value to check.
 * @param[in] expected_value Center of the expected range.
 * @param[in] margin         Allowed distance from the expected value.
 */
#define ASSERT_UINT_RANGE(value, expected_value, margin)                                                      \
    do                                                                                                        \
    {                                                                                                         \
        if (!_c_assert_uint_range(__func__, __FILE__, __LINE__, (uintmax_t)(value),                           \
                                  (uintmax_t)(expected_value), (uintmax_t)(margin), #value, #expected_value)) \
        {                                                                                                     \
            _ut_result = UT_FAILED;                                                                           \
            goto cleanup;                                                                                     \
        }                                                                                                     \
        _ut_result = UT_PASSED;                                                                               \
    } while (0)

/**
 * @brief Asserts that a pointer equals an expected pointer.
 *
 * @param[in] value          Pointer value to check.
 * @param[in] expected_value Expected pointer value.
 */
#define ASSERT_PTR(value, expected_value)                                                                       \
    do                                                                                                          \
    {                                                                                                           \
        if (!_c_assert_ptr(__func__, __FILE__, __LINE__, (const void *)(value), (const void *)(expected_value), \
                           #value, #expected_value))                                                            \
        {                                                                                                       \
            _ut_result = UT_FAILED;                                                                             \
            goto cleanup;                                                                                       \
        }                                                                                                       \
        _ut_result = UT_PASSED;                                                                                 \
    } while (0)

/**
 * @brief Asserts that two memory regions contain identical bytes.
 *
 * @param[in] buffer          Memory region to check.
 * @param[in] expected_buffer Expected memory region.
 * @param[in] size            Number of bytes to compare.
 *
 * @warning This assertion reads `size` bytes from both pointers.
 */
#define ASSERT_MEMORY(buffer, expected_buffer, size)                                               \
    do                                                                                             \
    {                                                                                              \
        if (!_c_assert_memory(__func__, __FILE__, __LINE__, (const void *)(buffer),                \
                              (const void *)(expected_buffer), (size), #buffer, #expected_buffer)) \
        {                                                                                          \
            _ut_result = UT_FAILED;                                                                \
            goto cleanup;                                                                          \
        }                                                                                          \
        _ut_result = UT_PASSED;                                                                    \
    } while (0)

/**
 * @brief Asserts that a subtest returned `UT_PASSED`.
 *
 * @param[in] subtest Expression returning `unit_test_result_t`.
 */
#define ASSERT_SUBTEST(subtest)                                                    \
    do                                                                             \
    {                                                                              \
        if (!_c_assert_subtest(__func__, __FILE__, __LINE__, (subtest), #subtest)) \
        {                                                                          \
            _ut_result = UT_FAILED;                                                \
            goto cleanup;                                                          \
        }                                                                          \
        _ut_result = UT_PASSED;                                                    \
    } while (0)

/**
 * @brief Soft assert that checks a generic boolean condition.
 *
 * Soft assertions report the failure but do not jump to `cleanup` or fail the
 * whole test immediately. This is useful when debugging multiple conditions in
 * one test and deciding the final `_ut_result` at the end.
 *
 * @param[in] condition Boolean condition to check.
 *
 * @return `true` when the condition is true, otherwise `false`.
 */
#define SOFT_ASSERT(condition) _c_assert_check(__func__, __FILE__, __LINE__, (condition), #condition)

/**
 * @brief Soft assert that checks a boolean value is true.
 *
 * @param[in] value Boolean expression or value to check.
 *
 * @return `true` if the value is true, otherwise `false`.
 */
#define SOFT_ASSERT_TRUE(value) _c_assert_true(__func__, __FILE__, __LINE__, (value), #value)

/**
 * @brief Soft assert that checks a boolean value is false.
 *
 * @param[in] value Boolean expression or value to check.
 *
 * @return `true` if the value is false, otherwise `false`.
 */
#define SOFT_ASSERT_FALSE(value) _c_assert_false(__func__, __FILE__, __LINE__, (value), #value)

/**
 * @brief Soft assert that checks a signed integer equals an expected value.
 *
 * @param[in] value          Signed integer value to check.
 * @param[in] expected_value Expected signed integer value.
 *
 * @return `true` when the values match, otherwise `false`.
 */
#define SOFT_ASSERT_INT(value, expected_value)                                                         \
    _c_assert_int(__func__, __FILE__, __LINE__, (intmax_t)(value), (intmax_t)(expected_value), #value, \
                  #expected_value)

/**
 * @brief Soft assert that checks an unsigned integer equals an expected value.
 *
 * @param[in] value          Unsigned integer value to check.
 * @param[in] expected_value Expected unsigned integer value.
 *
 * @return `true` when the values match, otherwise `false`.
 */
#define SOFT_ASSERT_UINT(value, expected_value)                                                           \
    _c_assert_uint(__func__, __FILE__, __LINE__, (uintmax_t)(value), (uintmax_t)(expected_value), #value, \
                   #expected_value)

/**
 * @brief Soft assert that checks a 64-bit unsigned integer equals an expected value.
 *
 * @param[in] value          64-bit unsigned integer value to check.
 * @param[in] expected_value Expected 64-bit unsigned integer value.
 *
 * @return `true` when the values match, otherwise `false`.
 */
#define SOFT_ASSERT_UINT64(value, expected_value)                                                         \
    _c_assert_uint64(__func__, __FILE__, __LINE__, (uint64_t)(value), (uint64_t)(expected_value), #value, \
                     #expected_value)

/**
 * @brief Soft assert that checks an unsigned integer is within a range.
 *
 * @param[in] value          Unsigned integer value to check.
 * @param[in] expected_value Center of the expected range.
 * @param[in] margin         Allowed distance from the expected value.
 *
 * @return `true` when the value is in range, otherwise `false`.
 */
#define SOFT_ASSERT_UINT_RANGE(value, expected_value, margin)                                           \
    _c_assert_uint_range(__func__, __FILE__, __LINE__, (uintmax_t)(value), (uintmax_t)(expected_value), \
                         (uintmax_t)(margin), #value, #expected_value)

/**
 * @brief Soft assert that checks a pointer equals an expected pointer.
 *
 * @param[in] value          Pointer value to check.
 * @param[in] expected_value Expected pointer value.
 *
 * @return `true` when the pointers match, otherwise `false`.
 */
#define SOFT_ASSERT_PTR(value, expected_value)                                                                 \
    _c_assert_ptr(__func__, __FILE__, __LINE__, (const void *)(value), (const void *)(expected_value), #value, \
                  #expected_value)

/**
 * @brief Soft assert that checks two memory regions contain identical bytes.
 *
 * @param[in] buffer          Memory region to check.
 * @param[in] expected_buffer Expected memory region.
 * @param[in] size            Number of bytes to compare.
 *
 * @return `true` when the memory matches, otherwise `false`.
 *
 * @warning This assertion reads `size` bytes from both pointers.
 */
#define SOFT_ASSERT_MEMORY(buffer, expected_buffer, size)                                                   \
    _c_assert_memory(__func__, __FILE__, __LINE__, (const void *)(buffer), (const void *)(expected_buffer), \
                     (size), #buffer, #expected_buffer)

/**
 * @brief Soft assert that checks a subtest returned `UT_PASSED`.
 *
 * @param[in] subtest Expression returning `unit_test_result_t`.
 *
 * @return `true` when the subtest passed, otherwise `false`.
 */
#define SOFT_ASSERT_SUBTEST(subtest) _c_assert_subtest(__func__, __FILE__, __LINE__, (subtest), #subtest)

/**
 * @brief Declares a static unit test function returning `unit_test_result_t`.
 */
#define TEST_JIG static unit_test_result_t

/**
 * @brief Declares a test group function.
 */
#define TEST_GROUP void

/**
 * @brief Declares a test suite function.
 */
#define TEST_SUITE void

/**
 * @brief Initializes a test result variable for a cleanup-aware test.
 *
 * Each `TEST_JIG` using hard assertions must call this before the first
 * assertion.
 */
#define UT_BUILDUP unit_test_result_t _ut_result = UT_NOT_IMPLEMENTED

/**
 * @brief Runs cleanup code and returns the current test result.
 *
 * Hard assertions jump to the `cleanup` label created by this macro. Cleanup
 * statements are optional.
 *
 * @param[in] ... Optional cleanup statements.
 */
#define UT_TEARDOWN(...)   \
    do                     \
    {                      \
        if (0)             \
        {                  \
            goto cleanup;  \
        }                  \
    cleanup:               \
        do                 \
        {                  \
            __VA_ARGS__    \
        } while (0);       \
        return _ut_result; \
    } while (0)

/**
 * @brief Executes a unit test function and records its result.
 *
 * @param[in] func Test function returning `unit_test_result_t`.
 */
#define TEST(func)                                               \
    do                                                           \
    {                                                            \
        _c_assert_run_test((func)(), __FILE__, __LINE__, #func); \
    } while (0)

/**
 * @brief Starts a unit test suite.
 *
 * Clears all result counters and resets internal output state.
 */
#define UT_START() _ut_start()

/**
 * @brief Prints the final test summary and returns the test run status.
 *
 * @return `0` when all executed tests passed, otherwise `-1`.
 */
#define UT_END() _display_test_summary()

/**
 * @brief Converts a path into a display-friendly file name.
 *
 * @param[in] file Path to shorten.
 *
 * @return Pointer to the file-name portion of `file`, or `"(null)"` if `file`
 * is `NULL`.
 */
const char *_c_assert_path(const char *file);

/**
 * @brief Starts a unit test suite and clears previous result counters.
 */
void _ut_start(void);

/**
 * @brief Checks a generic boolean condition and reports it when false.
 *
 * @param[in] func_name  Function where the check occurred.
 * @param[in] file       File where the check occurred.
 * @param[in] line       Line where the check occurred.
 * @param[in] condition  Condition to check.
 * @param[in] expression String form of the condition.
 *
 * @return `true` if the condition is true, otherwise `false`.
 */
bool _c_assert_check(const char *func_name, const char *file, int line, bool condition, const char *expression);

/**
 * @brief Checks that a subtest result is `UT_PASSED`.
 *
 * @param[in] func_name    Function where the check occurred.
 * @param[in] file         File where the check occurred.
 * @param[in] line         Line where the check occurred.
 * @param[in] subtest      Subtest result to check.
 * @param[in] subtest_name String form of the subtest expression.
 *
 * @return `true` if `subtest` is `UT_PASSED`, otherwise `false`.
 */
bool _c_assert_subtest(const char *func_name, const char *file, int line, unit_test_result_t subtest,
                       const char *subtest_name);

/**
 * @brief Checks that a boolean value is true.
 *
 * @param[in] func_name  Function where the check occurred.
 * @param[in] file       File where the check occurred.
 * @param[in] line       Line where the check occurred.
 * @param[in] value      Boolean value to check.
 * @param[in] value_name String form of the value expression.
 *
 * @return `true` if `value` is true, otherwise `false`.
 */
bool _c_assert_true(const char *func_name, const char *file, int line, bool value, const char *value_name);

/**
 * @brief Checks that a boolean value is false.
 *
 * @param[in] func_name  Function where the check occurred.
 * @param[in] file       File where the check occurred.
 * @param[in] line       Line where the check occurred.
 * @param[in] value      Boolean value to check.
 * @param[in] value_name String form of the value expression.
 *
 * @return `true` if `value` is false, otherwise `false`.
 */
bool _c_assert_false(const char *func_name, const char *file, int line, bool value, const char *value_name);

/**
 * @brief Checks that a signed integer equals an expected value.
 *
 * @param[in] func_name     Function where the check occurred.
 * @param[in] file          File where the check occurred.
 * @param[in] line          Line where the check occurred.
 * @param[in] value         Signed integer value to check.
 * @param[in] expected      Expected signed integer value.
 * @param[in] value_name    String form of `value`.
 * @param[in] expected_name String form of `expected`.
 *
 * @return `true` when the values match, otherwise `false`.
 */
bool _c_assert_int(const char *func_name, const char *file, int line, intmax_t value, intmax_t expected,
                   const char *value_name, const char *expected_name);

/**
 * @brief Checks that an unsigned integer equals an expected value.
 *
 * @param[in] func_name     Function where the check occurred.
 * @param[in] file          File where the check occurred.
 * @param[in] line          Line where the check occurred.
 * @param[in] value         Unsigned integer value to check.
 * @param[in] expected      Expected unsigned integer value.
 * @param[in] value_name    String form of `value`.
 * @param[in] expected_name String form of `expected`.
 *
 * @return `true` when the values match, otherwise `false`.
 */
bool _c_assert_uint(const char *func_name, const char *file, int line, uintmax_t value, uintmax_t expected,
                    const char *value_name, const char *expected_name);

/**
 * @brief Checks that a 64-bit unsigned integer equals an expected value.
 *
 * @param[in] func_name     Function where the check occurred.
 * @param[in] file          File where the check occurred.
 * @param[in] line          Line where the check occurred.
 * @param[in] value         64-bit unsigned integer value to check.
 * @param[in] expected      Expected 64-bit unsigned integer value.
 * @param[in] value_name    String form of `value`.
 * @param[in] expected_name String form of `expected`.
 *
 * @return `true` when the values match, otherwise `false`.
 */
bool _c_assert_uint64(const char *func_name, const char *file, int line, uint64_t value, uint64_t expected,
                      const char *value_name, const char *expected_name);

/**
 * @brief Checks that an unsigned integer is within an expected range.
 *
 * @param[in] func_name     Function where the check occurred.
 * @param[in] file          File where the check occurred.
 * @param[in] line          Line where the check occurred.
 * @param[in] value         Unsigned integer value to check.
 * @param[in] expected      Center of the expected range.
 * @param[in] margin        Allowed distance from `expected`.
 * @param[in] value_name    String form of `value`.
 * @param[in] expected_name String form of `expected`.
 *
 * @return `true` when the value is in range, otherwise `false`.
 */
bool _c_assert_uint_range(const char *func_name, const char *file, int line, uintmax_t value, uintmax_t expected,
                          uintmax_t margin, const char *value_name, const char *expected_name);

/**
 * @brief Checks that a pointer equals an expected pointer.
 *
 * @param[in] func_name     Function where the check occurred.
 * @param[in] file          File where the check occurred.
 * @param[in] line          Line where the check occurred.
 * @param[in] value         Pointer value to check.
 * @param[in] expected      Expected pointer value.
 * @param[in] value_name    String form of `value`.
 * @param[in] expected_name String form of `expected`.
 *
 * @return `true` when the pointers match, otherwise `false`.
 */
bool _c_assert_ptr(const char *func_name, const char *file, int line, const void *value, const void *expected,
                   const char *value_name, const char *expected_name);

/**
 * @brief Checks that two memory regions contain identical bytes.
 *
 * @param[in] func_name            Function where the check occurred.
 * @param[in] file                 File where the check occurred.
 * @param[in] line                 Line where the check occurred.
 * @param[in] buffer               Memory region to check.
 * @param[in] expected_buffer      Expected memory region.
 * @param[in] size                 Number of bytes to compare.
 * @param[in] buffer_name          String form of `buffer`.
 * @param[in] expected_buffer_name String form of `expected_buffer`.
 *
 * @return `true` when memory matches, otherwise `false`.
 *
 * @warning This function reads `size` bytes from both pointers.
 */
bool _c_assert_memory(const char *func_name, const char *file, int line, const void *buffer,
                      const void *expected_buffer, size_t size, const char *buffer_name,
                      const char *expected_buffer_name);

/**
 * @brief Reads a bit from a 32-bit value.
 *
 * @param[in] register_value Value to read from.
 * @param[in] bit_position   Zero-based bit position.
 *
 * @return Bit value, or `0` when `bit_position` is outside the 32-bit range.
 */
uint32_t _c_assert_read_bit(uint32_t register_value, uint8_t bit_position);

/**
 * @brief Reads a bit range from a 32-bit value.
 *
 * @param[in] register_value Value to read from.
 * @param[in] start_position Zero-based first bit position.
 * @param[in] length         Number of bits to read.
 *
 * @return Right-aligned bit range value, or `0` when the range is empty or
 * starts outside the 32-bit range.
 */
uint32_t _c_assert_read_bit_range(uint32_t register_value, uint8_t start_position, uint8_t length);

/**
 * @brief Records and prints one test result.
 *
 * @param[in] result    Test result returned by the test function.
 * @param[in] file      File where `TEST()` was called.
 * @param[in] line      Line where `TEST()` was called.
 * @param[in] test_name Name of the test function.
 */
void _c_assert_run_test(unit_test_result_t result, const char *file, int line, const char *test_name);

/**
 * @brief Prints the final test summary.
 *
 * @return `0` when all executed tests passed, otherwise `-1`.
 */
int _display_test_summary(void);

#endif // C_ASSERTS_H_

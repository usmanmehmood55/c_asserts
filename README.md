# C Asserts Library

The C Asserts Library is an assertion and unit test library written in C. The library
is designed to facilitate unit testing by providing straightforward macros
and utilities for testing conditions within C programs. If a test condition fails,
the library will output the file, line number, and function where the failure
occurred.

## 1. Features

- Write tests as `unit_test_result_t` functions with `TEST_JIG`, `UT_BUILDUP`, and `UT_TEARDOWN(...)`.
- Typed assertions for booleans, signed integers, unsigned integers, 64-bit unsigned integers, pointers, memory buffers, ranges, and subtests.
- Soft assertions that report failures without failing the whole test immediately.
- Automatic tracking of total, passed, failed, and not-implemented tests.
- Display test summary at the end of your test suite.
- Bit helper functions for embedded/register tests.

## 2. Requirements

- GCC
- CMake

## 3. Usage

Here's a quick example that demonstrates how to use the library:

```c
#include "c_asserts.h"

TEST_JIG passing_function_1(void)
{
    UT_BUILDUP;

    int a = 1;
    int b = 2;
    int c = a + b;

    ASSERT_INT(c, 3);

    UT_TEARDOWN();
}

TEST_JIG passing_function_2(void)
{
    UT_BUILDUP;

    char * string = "this is a string";

    ASSERT_TRUE(strcmp(string, "this is a string") == 0);

    UT_TEARDOWN();
}

TEST_JIG passing_function_3(void)
{
    UT_BUILDUP;

    uint8_t actual[] = { 0x01, 0x02, 0x03 };
    uint8_t expected[] = { 0x01, 0x02, 0x03 };

    ASSERT_MEMORY(actual, expected, sizeof(actual));

    UT_TEARDOWN();
}

TEST_JIG failing_function(void)
{
    UT_BUILDUP;

    ASSERT_UINT(1U, 2U);  // This will fail

    UT_TEARDOWN();
}

TEST_JIG cleanup_function(void)
{
    UT_BUILDUP;

    int resource = 123;

    ASSERT_INT(resource, 123);

    UT_TEARDOWN(
        resource = 0;
        (void)resource;
    );
}

TEST_JIG future_test(void)
{
    UT_BUILDUP;

    UT_TEARDOWN();
}

int main(void)
{
    UT_START();

    TEST(passing_function_1);
    TEST(passing_function_2);
    TEST(passing_function_3);
    TEST(failing_function);
    TEST(cleanup_function);
    TEST(future_test);

    return UT_END();
}
```

### 3.1. Output

![image](https://github.com/user-attachments/assets/ecd5c41a-e10e-4b52-8efb-edd77a63bc2c)

### 3.2. Assertions

Tests return `unit_test_result_t`. `UT_BUILDUP` initializes the result to
`UT_NOT_IMPLEMENTED`; each hard assertion sets it to `UT_PASSED` on success or
`UT_FAILED` and jumps to the `UT_TEARDOWN(...)` cleanup block on failure:

```c
TEST_JIG my_test(void)
{
    UT_BUILDUP;

    ASSERT_TRUE(is_ready);
    ASSERT_FALSE(has_error);
    ASSERT_INT(actual_count, expected_count);
    ASSERT_UINT(actual_flags, expected_flags);
    ASSERT_UINT64(actual_ticks, expected_ticks);
    ASSERT_UINT_RANGE(actual_mv, expected_mv, tolerance_mv);
    ASSERT_PTR(actual_ptr, expected_ptr);
    ASSERT_MEMORY(actual_buffer, expected_buffer, buffer_size);
    ASSERT_SUBTEST(helper_test());

    UT_TEARDOWN();
}
```

Soft assertions return a boolean and let the current test continue. Use them
when you want to check whether an assertion passes without failing the whole
test immediately, which is useful when debugging several related conditions in
one run. They report failures, keep executing, and let the test decide the
final result by setting `_ut_result` before teardown:

```c
bool passing = true;

passing &= SOFT_ASSERT_INT(actual_count, expected_count);
passing &= SOFT_ASSERT_MEMORY(actual_buffer, expected_buffer, buffer_size);
_ut_result = passing ? UT_PASSED : UT_FAILED;

UT_TEARDOWN();
```

### 3.3. Running Tests

Use `TEST(func)` to run a test function:

```c
TEST(my_test_function);
```

Use `UT_START()` at the beginning of the suite to clear previous results:

```c
UT_START();
```

Leave a `TEST_JIG` with only `UT_BUILDUP` and `UT_TEARDOWN()` to include a
planned test in the summary as not implemented:

```c
TEST_JIG test_error_timeout(void)
{
    UT_BUILDUP;

    UT_TEARDOWN();
}
```

### 3.4. Displaying Summary

At the end of your test suite, you can display the test summary:

```c
return UT_END();
```

This will print the total number of tests, passed tests, failed tests,
not-implemented tests, and pass percentage.

## 4. Example Application

The repository includes one comprehensive sample at
`examples/demo.c`. It runs passing assertions first, intentional
failures next, and not-implemented tests last. When this repository is built
directly, CMake creates the `c_asserts_demo` executable:

```bash
cmake -S . -B build
cmake --build build
./build/c_asserts_demo
```

## 5. Installation

To install the library, clone this repository as a submodule
git submodule in your project

```bash
git submodule add https://github.com/usmanmehmood55/c_asserts.git c_asserts
git submodule update --init --recursive
```

Then in your root CMakeLists.txt, add the library by including its subdirectory

```bash
add_subdirectory(c_asserts)
```

And also link it with the executable

```bash
target_link_libraries(${PROJECT_NAME} PRIVATE c_asserts_obj)
```

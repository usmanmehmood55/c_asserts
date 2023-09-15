# C Asserts Library

The C Asserts Library is a rudimentary assertion library written in C. The library 
is designed to facilitate simple unit testing by providing straightforward macros 
and utilities for testing conditions within C programs. If a test condition fails, 
the library will output the file, line number, and function where the failure 
occurred.

## Features
- Assert tests with the `ASSERT_TEST(condition)` macro.
- Automatic tracking of total, passed, and failed tests.
- Display test summary at the end of your test suite.

## Requirements
- GCC
- CMake

## Usage
Here's a quick example that demonstrates how to use the library:
```c
#include "c_asserts.h"

static bool passing_function_1(void)
{
    int a = 1;
    int b = 2;
    int c = a + b;

    ASSERT_TEST(c == 3);
    return true;
}

static bool passing_function_2(void)
{
    char * string = "this is a string";

    ASSERT_TEST(strcmp(string, "this is a string") == 0);;
    return true;
}

static bool failing_function(void)
{
    ASSERT_TEST(1 == 2);  // This will fail
    return true;
}

static bool passing_function_3(void)
{
    int upper_limit = 20;
    int lower_limit = 15;
    srand(time(NULL));
    int random_number = ((rand() % (upper_limit - lower_limit + 1)) + lower_limit);

    ASSERT_TEST(random_number <= upper_limit);
    ASSERT_TEST(random_number >= lower_limit);
    return true;
}

int main(void)
{
    TEST(passing_function_1);
    TEST(passing_function_2);
    TEST(failing_function);
    TEST(passing_function_3);

    return display_test_summary();
}
```

### Output
```bash
[PASSED] main.c:42 passing_function_1    
[PASSED] main.c:43 passing_function_2    
Assertion failed at main.c:24:failing_function
[FAILED] main.c:44 failing_function
[PASSED] main.c:45 passing_function_3

----- Test Summary -----
 Total tests:  04
 Passed tests: 03
 Failed tests: 01
------------------------
```

### Assertions
Use `ASSERT_TEST(condition)` to assert that a condition is true:
```c
ASSERT_TEST(x > 0);
```

### Running Tests
Use `TEST(func)` to run a test function:

```c
TEST(my_test_function);
```

### Displaying Summary
At the end of your test suite, you can display the test summary:
```c
return display_test_summary();
```

This will print the total number of tests, the number of passed tests, and the 
number of failed tests.

## Installation
To install the library, clone this repository as a submodule
git submodule in your project
```bash
git submodule add https://github.com/usmanmehmood55/c_asserts.git .
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
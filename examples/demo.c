#include "c_asserts.h"

TEST_JIG helper_subtest_passes(void)
{
    UT_BUILDUP;

    ASSERT_TRUE(true);
    ASSERT_INT(2 + 2, 4);

    UT_TEARDOWN();
}

TEST_JIG all_hard_asserts_pass(void)
{
    UT_BUILDUP;

    int signed_value = -12;
    uint32_t flags = 0xA5U;
    uint64_t tick_count = 123456789ULL;
    int value = 42;
    int *actual_ptr = &value;
    uint8_t actual_buffer[] = { 0x10U, 0x20U, 0x30U, 0x40U };
    uint8_t expected_buffer[] = { 0x10U, 0x20U, 0x30U, 0x40U };

    ASSERT_TRUE(signed_value < 0);
    ASSERT_TRUE(flags != 0U);
    ASSERT_FALSE(flags == 0U);
    ASSERT_INT(signed_value, -12);
    ASSERT_UINT(flags, 0xA5U);
    ASSERT_UINT64(tick_count, 123456789ULL);
    ASSERT_UINT_RANGE(1000U, 1005U, 5U);
    ASSERT_PTR(actual_ptr, &value);
    ASSERT_MEMORY(actual_buffer, expected_buffer, sizeof(actual_buffer));
    ASSERT_SUBTEST(helper_subtest_passes());

    UT_TEARDOWN();
}

TEST_JIG all_soft_asserts_pass(void)
{
    UT_BUILDUP;

    bool passing = true;
    int value = 42;
    int *actual_ptr = &value;
    uint8_t actual_buffer[] = { 0x10U, 0x20U, 0x30U, 0x40U };
    uint8_t expected_buffer[] = { 0x10U, 0x20U, 0x30U, 0x40U };

    passing &= SOFT_ASSERT_TRUE(2 + 2 == 4);
    passing &= SOFT_ASSERT_TRUE(true);
    passing &= SOFT_ASSERT_FALSE(false);
    passing &= SOFT_ASSERT_INT(-12, -12);
    passing &= SOFT_ASSERT_UINT(0xA5U, 0xA5U);
    passing &= SOFT_ASSERT_UINT64(123456789ULL, 123456789ULL);
    passing &= SOFT_ASSERT_UINT_RANGE(1000U, 1005U, 5U);
    passing &= SOFT_ASSERT_PTR(actual_ptr, &value);
    passing &= SOFT_ASSERT_MEMORY(actual_buffer, expected_buffer, sizeof(actual_buffer));
    passing &= SOFT_ASSERT_SUBTEST(helper_subtest_passes());
    _ut_result = passing ? UT_PASSED : UT_FAILED;

    UT_TEARDOWN();
}

TEST_JIG cleanup_teardown_passes(void)
{
    UT_BUILDUP;

    bool cleanup_ran = false;
    int resource = 123;

    ASSERT_INT(resource, 123);

    UT_TEARDOWN(
        cleanup_ran = true;
        resource = 0;
        (void)cleanup_ran;
        (void)resource;
    );
}

TEST_JIG assert_true_fails(void)
{
    UT_BUILDUP;

    ASSERT_TRUE(false);

    UT_TEARDOWN();
}

TEST_JIG assert_false_fails(void)
{
    UT_BUILDUP;

    ASSERT_FALSE(true);

    UT_TEARDOWN();
}

TEST_JIG assert_int_fails(void)
{
    UT_BUILDUP;

    ASSERT_INT(-4, 4);

    UT_TEARDOWN();
}

TEST_JIG assert_uint_fails(void)
{
    UT_BUILDUP;

    ASSERT_UINT(0x10U, 0x20U);

    UT_TEARDOWN();
}

TEST_JIG assert_uint64_fails(void)
{
    UT_BUILDUP;

    ASSERT_UINT64(10000000000ULL, 10000000001ULL);

    UT_TEARDOWN();
}

TEST_JIG assert_uint_range_fails(void)
{
    UT_BUILDUP;

    ASSERT_UINT_RANGE(90U, 100U, 5U);

    UT_TEARDOWN();
}

TEST_JIG assert_ptr_fails(void)
{
    UT_BUILDUP;

    int actual = 1;
    int expected = 1;

    ASSERT_PTR(&actual, &expected);

    UT_TEARDOWN();
}

TEST_JIG assert_memory_fails(void)
{
    UT_BUILDUP;

    uint8_t actual[] = { 0xAAU, 0xBBU, 0xCCU, 0xDDU };
    uint8_t expected[] = { 0xAAU, 0xBBU, 0x00U, 0xDDU };

    ASSERT_MEMORY(actual, expected, sizeof(actual));

    UT_TEARDOWN();
}

TEST_JIG assert_subtest_fails(void)
{
    UT_BUILDUP;

    ASSERT_SUBTEST(UT_FAILED);

    UT_TEARDOWN();
}

TEST_JIG all_soft_asserts_fail(void)
{
    UT_BUILDUP;

    bool passing = true;
    int actual = 7;
    int expected = 8;
    uint8_t actual_buffer[] = { 0x01U, 0x02U };
    uint8_t expected_buffer[] = { 0x01U, 0x03U };

    passing &= SOFT_ASSERT_TRUE(1 == 0);
    passing &= SOFT_ASSERT_TRUE(false);
    passing &= SOFT_ASSERT_FALSE(true);
    passing &= SOFT_ASSERT_INT(-1, 1);
    passing &= SOFT_ASSERT_UINT(10U, 11U);
    passing &= SOFT_ASSERT_UINT64(20ULL, 21ULL);
    passing &= SOFT_ASSERT_UINT_RANGE(30U, 40U, 5U);
    passing &= SOFT_ASSERT_PTR(&actual, &expected);
    passing &= SOFT_ASSERT_MEMORY(actual_buffer, expected_buffer, sizeof(actual_buffer));
    passing &= SOFT_ASSERT_SUBTEST(UT_FAILED);
    _ut_result = passing ? UT_PASSED : UT_FAILED;

    UT_TEARDOWN();
}

TEST_JIG cleanup_teardown_fails(void)
{
    UT_BUILDUP;

    bool cleanup_ran = false;
    int resource = 123;

    ASSERT_INT(resource, 456);

    UT_TEARDOWN(
        cleanup_ran = true;
        resource = 0;
        (void)cleanup_ran;
        (void)resource;
    );
}

TEST_JIG not_implemented_placeholder(void)
{
    UT_BUILDUP;
    // Since there's no asserts here, this will show as "not implemented".
    UT_TEARDOWN();
}

TEST_JIG future_timeout_test(void)
{
    UT_BUILDUP;
    // Since there's no asserts here, this will show as "not implemented".
    UT_TEARDOWN();
}

TEST_JIG future_error_path_test(void)
{
    UT_BUILDUP;
    // Since there's no asserts here, this will show as "not implemented".
    UT_TEARDOWN();
}

int main(void)
{
    UT_START();

    TEST(all_hard_asserts_pass);
    TEST(all_soft_asserts_pass);
    TEST(cleanup_teardown_passes);

    TEST(assert_true_fails);
    TEST(assert_false_fails);
    TEST(assert_int_fails);
    TEST(assert_uint_fails);
    TEST(assert_uint64_fails);
    TEST(assert_uint_range_fails);
    TEST(assert_ptr_fails);
    TEST(assert_memory_fails);
    TEST(assert_subtest_fails);
    TEST(all_soft_asserts_fail);
    TEST(cleanup_teardown_fails);

    TEST(not_implemented_placeholder);
    TEST(future_timeout_test);
    TEST(future_error_path_test);

    return UT_END();
}

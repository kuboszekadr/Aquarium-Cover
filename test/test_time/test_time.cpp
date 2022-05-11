#include "../lib/Lighting/Time.hpp"

#include <unity.h>

void test_epoch()
{
    Time t = Time(0, 0, 0);
    uint32_t actual = t.epochs();

    TEST_ASSERT_EQUAL(0, actual);

    t = Time(24, 0, 0);
    actual = t.epochs();

    TEST_ASSERT_EQUAL(86400, actual);

    t = Time(0, 12, 0);
    actual = t.epochs();

    TEST_ASSERT_EQUAL(720, actual);

    t = {23, 30, 0};
    actual = t.epochs();

    TEST_ASSERT_EQUAL(84600, actual);
}

void test_time()
{
  Time t = Time(160000);
  TEST_ASSERT_EQUAL(16, t.hour);
  TEST_ASSERT_EQUAL(0, t.minute);
  TEST_ASSERT_EQUAL(0, t.seconds);

  t = Time(170159);
  TEST_ASSERT_EQUAL(17, t.hour);
  TEST_ASSERT_EQUAL(1, t.minute);
  TEST_ASSERT_EQUAL(59, t.seconds);

  t = Time(0);
  TEST_ASSERT_EQUAL(0, t.hour);
  TEST_ASSERT_EQUAL(0, t.minute);
  TEST_ASSERT_EQUAL(0, t.seconds);

  t = Time(30);
  TEST_ASSERT_EQUAL(0, t.hour);
  TEST_ASSERT_EQUAL(0, t.minute);
  TEST_ASSERT_EQUAL(30, t.seconds);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_epoch);
    RUN_TEST(test_time);

    UNITY_END();
}
#include "../lib/Lighting/Program/Program.h"
#include "../lib/Lighting/Program/Program.cpp"

#include <unity.h>
#include <string.h>
#include <stdio.h>

using namespace Lighting;

// Program daylight
Color color_daylight = {100, 100, 100};
auto day_light = Program(
    "daylight",
    1200,
    1500,
    color_daylight,
    color_daylight);

// Program dawn
auto dawn = Program(
    "dawn",
    900,
    1200,
    (Color){0, 0, 0},
    color_daylight);

// Program dusk
auto dusk = Program(
    "dusk",
    1500,
    1800,
    color_daylight,
    (Color){50, 50, 50});

void test_programsSize()
{
    TEST_ASSERT_EQUAL(3, programs.size());
}

void test_getColor()
{
    Time t = Time(18, 0, 0);
    uint32_t actual = dusk.getColor(t.epochs());
    TEST_ASSERT_EQUAL(3289650, actual);

    actual = dusk.getColor(150000);
    TEST_ASSERT_EQUAL(6579300, actual);

    actual = dusk.getColor(163000);
    TEST_ASSERT_EQUAL(4934475, actual);
}

void test_progress()
{
    Time t = Time(9, 0, 0);
    float progress = dawn.progress(t.epochs(), 0);
    TEST_ASSERT_EQUAL_FLOAT(0, progress);

    t = Time(12, 0, 0);
    progress = dawn.progress(t.epochs(), 0);
    TEST_ASSERT_EQUAL_FLOAT(1, progress);

    t = Time(16, 30, 0);
    progress = dusk.progress(t.epochs(), 0);
    TEST_ASSERT_EQUAL_FLOAT(0.5, progress);

    t = Time(16, 21, 0);
    progress = dusk.progress(t.epochs(), 0);
    TEST_ASSERT_EQUAL_FLOAT(0.45, progress);
}

void test_isExecutable()
{
    Time t = Time(15, 0, 0);
    bool result = dusk.isExecutable(t.epochs());
    TEST_ASSERT_TRUE(result);

    t = Time(17, 59, 0);
    result = dusk.isExecutable(t.epochs());
    TEST_ASSERT_TRUE(result);

    t = Time(18, 1, 0);
    result = dusk.isExecutable(t.epochs());
    TEST_ASSERT_FALSE(result);
}

void test_getProgramToRun()
{
    Time t = Time(9, 0, 0);
    char *actual = getProgramToRun(t.epochs())->name();
    TEST_ASSERT_EQUAL_STRING("dawn", actual);

    t = Time(12, 0, 0);
    actual = getProgramToRun(t.epochs())->name();
    TEST_ASSERT_EQUAL_STRING("dawn", actual);

    t = Time(12, 1, 0);
    actual = getProgramToRun(t.epochs())->name();
    TEST_ASSERT_EQUAL_STRING("daylight", actual);

    t = Time(15, 0, 0);
    actual = getProgramToRun(t.epochs())->name();
    TEST_ASSERT_EQUAL_STRING("daylight", actual);

   t = Time(17, 0, 0);
    actual = getProgramToRun(t.epochs())->name();
    TEST_ASSERT_EQUAL_STRING("dusk", actual);

    t = Time(18, 1, 0);
    TEST_ASSERT_TRUE(getProgramToRun(t.epochs()) == nullptr);

    t = Time(8, 59, 0);
    TEST_ASSERT_TRUE(getProgramToRun(t.epochs()) == nullptr);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_programsSize);
    RUN_TEST(test_progress);
    RUN_TEST(test_isExecutable);
    RUN_TEST(test_getProgramToRun);

    UNITY_END();
}
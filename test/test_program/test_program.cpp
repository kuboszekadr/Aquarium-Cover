#include "../lib/Lighting/Program/Program.h"
#include "../lib/Lighting/Program/Program.cpp"

#include <unity.h>

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
    uint32_t actual = dusk.getColor(1800);
    TEST_ASSERT_EQUAL(3289650, actual);

    actual = dusk.getColor(1500);
    TEST_ASSERT_EQUAL(6579300, actual);

    actual = dusk.getColor(1630);
    TEST_ASSERT_EQUAL(4934475, actual);
}

void test_progress()
{
    float progress = dawn.progress(900, 0);
    TEST_ASSERT_EQUAL(0, progress);

    progress = dawn.progress(1200, 0);
    TEST_ASSERT_EQUAL(1, progress);

    progress = dusk.progress(1630, 0);
    TEST_ASSERT_EQUAL(0.5, progress);
}

void test_isExecutable()
{
    bool result = dusk.isExecutable(1500);
    TEST_ASSERT_TRUE(result);

    result = dusk.isExecutable(1759);
    TEST_ASSERT_TRUE(result);

    result = dusk.isExecutable(1801);
    TEST_ASSERT_FALSE(result);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_programsSize);
    RUN_TEST(test_progress);
    RUN_TEST(test_isExecutable);

    UNITY_END();
}
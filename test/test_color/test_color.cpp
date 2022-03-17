#include "../lib/Lighting/Program/Color.hpp"

#include <unity.h>

void test_color()
{
    Lighting::Color c = {255, 255, 255};

    TEST_ASSERT_TRUE(c.red == 255);
}

void test_comparison()
{
    Lighting::Color c = {156, 154, 102};
    TEST_ASSERT_TRUE(c == c);

    Lighting::Color b = {10, 10, 10};
    TEST_ASSERT_FALSE(c == b);
}

void test_multiplication()
{
    Lighting::Color c = {80, 70, 60};
    float mult = 0.6;

    Lighting::Color result = c * mult;

    Lighting::Color test = {48, 42, 36};
    TEST_ASSERT_TRUE(result == test);

    Lighting::Color test_false = {50, 50, 50};
    TEST_ASSERT_FALSE(result == test_false);
}

void test_addition()
{
    Lighting::Color a = {50, 50, 50};
    Lighting::Color b = {10, 150, 250};

    Lighting::Color test = a + b;
    Lighting::Color expected = {60, 200, 44};

    TEST_ASSERT_TRUE(test == expected);
}

void test_substraction()
{
    Lighting::Color a = {50, 50, 50};
    Lighting::Color b = {10, 150, 250};

    Lighting::Color test = b - a;
    Lighting::Color expected = {216, 100, 200};

    TEST_ASSERT_TRUE(test == expected);
}

void test_toPixelColor()
{
    Lighting::Color c = {0, 0, 0};
    uint32_t test = c.toPixelColor();

    TEST_ASSERT_EQUAL(0, test);

    c = {10, 250, 20};
    test = c.toPixelColor();
    TEST_ASSERT_EQUAL(16386580, test);

}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_comparison);
    RUN_TEST(test_color);
    RUN_TEST(test_multiplication);
    RUN_TEST(test_addition);
    RUN_TEST(test_substraction);
    RUN_TEST(test_toPixelColor);

    UNITY_END();
}
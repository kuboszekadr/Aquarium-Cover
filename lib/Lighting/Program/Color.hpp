#ifndef Lighting_Color_h
#define Lighting_Color_h

#include <Arduino.h>

namespace Lighting
{

    struct Color
    {
        uint8_t red;
        uint8_t blue;
        uint8_t white;

        uint32_t toPixelColor()
        {
            uint32_t color = (blue << 16) | (red << 8) | white;
            return color;
        }

        Color operator*(float scalar)
        {
            red *= scalar;
            blue *= scalar;
            white *= scalar;

            return *this;
        };

        Color operator-(Color color)
        {
            Color result = {0, 0, 0};

            result.red = red - color.red;
            result.white = white - color.white;
            result.blue = blue - color.blue;

            return result;
        };

        Color operator+(Color color)
        {
            Color result = {0, 0, 0};

            result.red = red + color.red;
            result.white = white + color.white;
            result.blue = blue + color.blue;

            return result;
        }
    };

}

#endif
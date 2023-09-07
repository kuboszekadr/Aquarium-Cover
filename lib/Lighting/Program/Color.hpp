#ifndef Lighting_Color_h
#define Lighting_Color_h

#include <cstdint>

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

        static Color fromPixelColor(uint32_t color)
        {
            Color result = {0, 0, 0};

            result.blue = (color >> 16) & 0xFF;
            result.red = (color >> 8) & 0xFF;
            result.white = color & 0xFF;    

            return result;          
        }

        Color operator*(float scalar)
        {
            Color result = {0, 0, 0};

            result.red = red * scalar;
            result.blue = blue * scalar;
            result.white = white * scalar;

            return result;
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
        };

        bool operator==(Color color)
        {
            bool result = true;

            result = result & (this->red == color.red); 
            result = result & (this->blue == color.blue); 
            result = result & (this->white == color.white);

            return result; 
        };        
    };

}

#endif
#ifndef Lighting_Program_h
#define Lighting_Program_h

#include "Color.hpp"

#include <map>
#include <array>

namespace Lighting
{
    class Program
    {
    public:
        Program(const char *name,
                uint32_t start_time,
                uint32_t end_time,
                Color color_start,
                Color color_end);

        uint32_t getColor(uint32_t timestamp, uint32_t offset);

        bool isExecutable(uint32_t timestamp, uint32_t offset);
        float progress(uint32_t timestamp, uint32_t offset);

    private:
        uint32_t _start_time;
        uint32_t _end_time;

        Color _color_start;
        Color _color_diff;
    };
    extern std::map<const char *, Program *> programs;

}

#endif
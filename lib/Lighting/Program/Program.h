#ifndef Lighting_Program_h
#define Lighting_Program_h

#include "Color.hpp"
#include "../Time.hpp"

#include <map>
#include <array>
#include <cstring>
#include <sys/time.h>

#ifdef ARDUINO
    #include <Arduino.h>
#else
    // #include <ArduinoFake.h>
#endif

namespace Lighting
{
    class Program
    {
    public:
        Program(const char *name,
                Time start_time,
                Time end_time,
                Color color_start,
                Color color_end);

        uint32_t getColor(uint32_t timestamp, uint32_t offset = 0);
        char *name() {return _name;};

        bool isExecutable(uint32_t timestamp, uint32_t offset = 0);
        float progress(uint32_t timestamp, uint32_t offset = 0);

        uint32_t _start_time;
        uint32_t _end_time;
    private:

        char _name[12];

        Color _color_start;
        Color _color_diff;
    };
    extern uint8_t programs_amount;
    extern Program *programs[10];

    Program *getProgramToRun(uint32_t timestamp, uint32_t offset = 0);
}

#endif
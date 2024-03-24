#ifndef Demo_h
#define Demo_h

#define DAY_DURATION_IN_SECONDS  8640000

#include "Lighting.h"
#include "Timestamp/Timestamp.h"
#include "Time.hpp"

namespace Lighting
{
    class Demo
    {
    public:
        Demo(Time start, Time end, uint32_t duration);

        std::vector<uint32_t> getSteps();
        uint32_t runStep(uint32_t timestamp);

        uint32_t _duration;
        uint32_t _step;

        record pixel;
        
        Time _start;
        Time _end;


    };
}
#endif
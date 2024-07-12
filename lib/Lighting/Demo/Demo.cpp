#include "Demo.h"

/**
 * @brief Constructs a Demo object with specified start time, end time, and duration.
 * 
 * @param start The start time of the demo.
 * @param end The end time of the demo.
 * @param duration The duration of the demo in minutes.
 */
Lighting::Demo::Demo(Time start, Time end, uint32_t duration)
{
    _duration = duration * 60 * 1000; // in miliseconds
    _start = start;
    _end = end;

    _step = 30 * 1000;
}


uint32_t Lighting::Demo::runStep(uint32_t timestamp)
{
    pixel = Lighting::loopOverCovers(timestamp)[0][1];

    uint32_t result = timestamp + _step;
    return result;
}
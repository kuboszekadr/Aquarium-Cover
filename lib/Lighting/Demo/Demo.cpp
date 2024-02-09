#include "Demo.h"

Lighting::Demo::Demo(uint32_t duration)
{
    _duration = duration * 60 * 100; // in seconds
    _step = DAY_DURATION_IN_SECONDS / _duration;
}

uint32_t Lighting::Demo::get_ts(uint32_t curr_millis)
{
    uint32_t delta_timestamp = curr_millis - _last_timestamp;
    delta_timestamp *= _step;

    uint32_t result = delta_timestamp;
    return result;
}

void Lighting::Demo::run(uint32_t start, uint32_t end)
{
    uint32_t timestamp = start;

    while (timestamp < end)
    {
        uint32_t curr_millis = getMillis(); 
        
        timestamp = get_ts(curr_millis);
        _last_timestamp = curr_millis;
        
        Lighting::loopOverCovers(timestamp);
        yield();
    }
}
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

    uint32_t result = EPOCH_202101010000 + delta_timestamp;
    return result;
}

void Lighting::Demo::run()
{
    uint32_t timestamp = EPOCH_202101010000;

    while (timestamp < EPOCH_202101020000)
    {
        ESP32Time ts;
        uint32_t curr_millis = ts.getMillis(); 
        
        timestamp = get_ts(curr_millis);
        _last_timestamp = curr_millis;
        
        Lighting::loop(timestamp);
        yield();
    }
}
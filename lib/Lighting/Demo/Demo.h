#ifndef Demo_h
#define Demo_h

#define DAY_DURATION_IN_SECONDS  8640000
#define EPOCH_202101010000 0
#define EPOCH_202101020000 8640000

#include "Lighting.h"

namespace Lighting
{
    class Demo
    {
    public:
        Demo(uint32_t duration);
        void run();
        uint32_t get_ts(uint32_t curr_millis);

    private:
        uint32_t _start_time;
        uint32_t _duration;
        uint32_t _step;
        uint32_t _last_timestamp = EPOCH_202101010000;
    };
}
#endif
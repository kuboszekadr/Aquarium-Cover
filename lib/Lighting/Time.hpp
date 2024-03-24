#ifndef Time_h
#define Time_h

#include <cstdint>
#include <ctime>

struct Time
{
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;

    uint16_t millis;

    Time()
    {
        hour = 0;
        minute = 0;
        seconds = 0;
        millis = 0;
    }

    Time(uint8_t hour, uint8_t minute, uint8_t seconds, uint32_t millis = 0)
    {
        this->hour = hour;
        this->minute = minute;
        this->seconds = seconds;
        this->millis = millis;
    }

    Time(uint32_t value)
    {
        hour = value / 60 / 60 / 1000;
        value %= 60 * 60 * 1000;

        minute = value / 60 / 1000;
        value = value % 60 * 1000;

        seconds = value / 1000;
        millis = value % 1000;
    }

    Time operator+(const Time& t2) const
    {
        uint32_t totalSeconds = this->toMillis() + t2.toMillis();
        return Time(totalSeconds);
    }

    Time operator-(const Time& t2) const
    {
        uint32_t totalSeconds = this->toMillis() - t2.toMillis();
        return Time(totalSeconds);
    }   

    uint32_t toMillis() const
    {
        uint32_t result = hour * 60 * 60 * 1000;
        result += (minute * 60 * 1000);
        result += seconds * 1000;
        result = result + millis;
        return result;
    }
};

#endif
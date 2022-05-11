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

  Time(uint8_t hour, uint8_t minute, uint8_t seconds, uint8_t millis = 0)
  {
    this->hour = hour;
    this->minute = minute;
    this->seconds = seconds;
    this->millis = millis;
  }

  Time(uint32_t timestamp)
  {
    hour = timestamp / 10000;
    minute = (timestamp - hour * 10000)/100;
    seconds = timestamp - hour * 10000 - minute * 100;
  }

  uint32_t epochs(bool ms = false)
  {
    uint32_t result = hour * 60 * 60;
    result += minute * 60;
    result += seconds;

    if (ms)
    {
      result = result * 100 + millis;
    }

    return result;
  }
};

#endif
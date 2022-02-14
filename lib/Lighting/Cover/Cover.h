#ifndef Cover_h
#define Cover_h

#include <Adafruit_NeoPixel.h>
#include <vector>

namespace Lighting
{
    class Cover : public Adafruit_NeoPixel
    {
    public:
        static std::vector<Cover> covers;

        Cover(uint8_t order, uint16_t pin, uint16_t pixels_amount);
        void start();

        uint8_t order() { return _order; };

    private:
        uint8_t _order = 0;
    };
}

#endif
#include "Cover.h"

Lighting::Cover::Cover(uint8_t order, uint16_t pin, uint16_t pixels_amount) : Adafruit_NeoPixel(pixels_amount, pin, NEO_GRB + NEO_KHZ800)
{
    _order = order;
}

void Lighting::Cover::start()
{
    begin();
    clear();
}

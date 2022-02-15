#include "Cover.h"

std::vector<Lighting::Cover*> Lighting::covers = std::vector<Lighting::Cover*>();

Lighting::Cover::Cover(uint8_t order, uint16_t pin, uint16_t pixels_amount) : Adafruit_NeoPixel(pixels_amount, pin, NEO_GRB + NEO_KHZ800)
{
    _order = order;
    covers.push_back(this);
}

void Lighting::Cover::start()
{
    begin();
    clear();
}

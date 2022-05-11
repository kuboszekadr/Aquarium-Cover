#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

int pin = 18;
uint8_t bright = 50;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(5, pin, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    Serial.begin(115200);
}

void loop()
{
    bright += 1;
    Serial.println(bright);
    for (uint8_t i = 0; i < 5; i++)
    {
        pixels.setPixelColor(i, bright, bright, bright);
    }
    pixels.show();
    delay(1000);
}
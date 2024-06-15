#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

int pin = 19;
uint8_t bright = 50;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(7, pin, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
    Serial.begin(115200);
}

void loop()
{
    Serial.println("1");
    for (uint8_t i = 0; i < 7; i++)
    {
        pixels.setPixelColor(i, 0, 0, 200);
    }
    pixels.show();
    delay(10000);

    Serial.println("2");
    for (uint8_t i = 0; i < 7; i++)
    {
        pixels.setPixelColor(i, 200, 0, 0);
    }
    pixels.show();
    delay(10000);

    Serial.println("3");
    for (uint8_t i = 0; i < 7; i++)
    {
        pixels.setPixelColor(i, 0, 200, 0);
    }
    pixels.show();
    delay(10000);
}

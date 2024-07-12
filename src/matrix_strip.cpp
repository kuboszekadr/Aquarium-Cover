#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <vector>

int pin = 19;
uint8_t bright = 50;
uint8_t cover_size = 14;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
    cover_size,
    pin,
    NEO_GRB + NEO_KHZ800);

struct Color
{
    uint8_t r;
    uint8_t w;
    uint8_t b;
};

std::vector<Color> colors = std::vector<Color>();

uint8_t pixel = 0;
uint8_t _color = 0;

String colorToString(const Color &color)
{
    return "Color(R: " + String(color.r) + ", W: " + String(color.w) + ", B: " + String(color.b) + ")";
}

void setup()
{
    pixels.begin();
    Serial.begin(115200);

    for (uint8_t i = 0; i < cover_size; i++)
    {
        colors.push_back(Color());
    }

    for (auto &color : colors)
    {
        color.r = 18;
        color.w = 18;
        color.b = 18;
    }
    Serial.println("Starting loop");
}
void loop()
{
    for (uint8_t i = 0; i < cover_size; i++)
    {
        Serial.print(i);
        Serial.print(":");
        pixels.setPixelColor(
            i,
            pixels.Color(colors[i].r,
                         colors[i].w,
                         colors[i].b));

        colors[i].r = (colors[i].r + 20) % 200;
        colors[i].w = (colors[i].w + 20) % 200;
        colors[i].b = (colors[i].b + 20) % 200;
    }
    pixels.show();
    delay(1000);

    // pixel = (++pixel) % cover_size;
    // _color = (++_color) % 3;
}

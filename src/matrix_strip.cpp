#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <vector>

int pin = 19;
uint8_t bright = 50;
uint8_t cover_size = 7;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
    cover_size, 
    pin, 
    NEO_GRB + NEO_KHZ800
    );

struct Color {
    uint8_t r;
    uint8_t w;
    uint8_t b;
};

std::vector<Color> colors = std::vector<Color>();

uint8_t pixel= 0;
uint8_t _color = 0;

String colorToString(const Color& color) {
    return "Color(R: " + String(color.r) + ", W: " + String(color.w) + ", B: " + String(color.b) + ")";
}

void setup()
{
    pixels.begin();
    Serial.begin(115200);

    for(uint8_t i = 0; i < cover_size; i++) {
        colors.push_back(Color());
    }

    for(auto& color : colors) {
        color.r = 0;
        color.w = 0;
        color.b = 0;
    }
}
void loop()
{
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        if (input.length() > 0) {
            char color = input.charAt(0);
            uint8_t value = 50;
            
            if (input.length() > 2) 
            {
                value = input.substring(2).toInt();
            }

            for (uint8_t pixel=0; pixel < cover_size; pixel++)
            {            

                switch (color) {
                    case 'r':
                        colors[pixel].r = value;
                        break;
                    case 'b':
                        colors[pixel].b = value;
                        break;
                    case 'w':
                        colors[pixel].w = value;
                        break;
                    default:
                        break;
                }
            }
        }
        Serial.println(colorToString(colors[pixel]));
    }

    for (uint8_t i=0; i < cover_size; i++)
    {
        // Serial.println(i);
        pixels.setPixelColor(
            i, 
            pixels.Color(colors[i].r, 
            colors[i].w, 
            colors[i].b)
            );
    }
    pixels.show();

    // pixel = (++pixel) % cover_size;
    // _color = (++_color) % 3;
    delay(2000);
}

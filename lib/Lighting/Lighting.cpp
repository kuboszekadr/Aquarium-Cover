#include "Lighting.h"

void Lighting::loop()
{
    ESP32Time ts;
    uint32_t timestamp = ts.getTime("%F%X").toInt();

    for (auto &cover : Cover::covers)
    {
        loopCover(cover, timestamp);
    }
}

void Lighting::loopCover(Cover &cover, uint32_t timestamp)
{
    uint32_t offset = LIGHTING_PROGRAM_OFFSET * (cover.order() - 1);
    for (uint8_t pixel = 0; pixel < cover.numPixels(); pixel++)
    {
        Program *program = getProgramToRun(timestamp, offset);

        uint32_t color = 0;
        if (program != nullptr)
        {
            color = program->getColor(timestamp, offset);
        }
        
        cover.setPixelColor(pixel, color);
        offset += LIGHTING_PROGRAM_OFFSET;
    }
}

Lighting::Program *Lighting::getProgramToRun(uint32_t timestamp, uint32_t offset)
{
    // for (const auto *program : programs)
    // {
    //     if (program->isExecutable(timestamp, offset))
    //     {
    //         return program;
    //     }
    // }
    return nullptr;
}
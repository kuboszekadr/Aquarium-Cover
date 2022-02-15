#include "Lighting.h"

void Lighting::loop()
{
    ESP32Time ts;
    uint32_t timestamp = ts.getTime("%F%X").toInt();

    for (auto &cover : covers)
    {
        loopCover(cover, timestamp);
    }
}

void Lighting::loopCover(Cover *cover, uint32_t timestamp)
{
    uint32_t offset = LIGHTING_PROGRAM_OFFSET * (cover->order() - 1);
    for (uint8_t pixel = 0; pixel < cover->numPixels(); pixel++)
    {
        Program *program = getProgramToRun(timestamp, offset);

        uint32_t color = 0;
        if (program != nullptr)
        {
            color = program->getColor(timestamp, offset);
        }

        cover->setPixelColor(pixel, color);
        offset += LIGHTING_PROGRAM_OFFSET;
    }
}

Lighting::Program *Lighting::getProgramToRun(uint32_t timestamp, uint32_t offset)
{
    for (const auto &program : programs)
    {
        if (program.second->isExecutable(timestamp, offset))
        {
            return program.second;
        }
    }
    return nullptr;
}

void Lighting::begin()
{
    for (auto &cover : covers)
    {
        cover->begin();
    }
}

void Lighting::setup()
{
    const char root_path[32] = "/config/lighting_programs";
    
    File root = SPIFFS.open(root_path);
    File file = root.openNextFile();

    while (file)
    {
        file = root.openNextFile();

        Config config = Config(file.name(), root_path);
        config.load();

        auto data = config.data;

        auto data_color_start = data["start"];

        // Program(file_name,
        //         data["start_time"].as<int>(),
        //         data["end_time"].as<int>(),
        //         );
    }
}
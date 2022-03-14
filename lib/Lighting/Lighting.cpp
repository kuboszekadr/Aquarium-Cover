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

void Lighting::begin()
{
    for (auto &cover : covers)
    {
        cover->begin();
    }
}

void Lighting::setup()
{
    const char root_path[32] = "/config/lighting";

    File root = SPIFFS.open(root_path);
    File file = root.openNextFile();

    while (file)
    {
        Config config = Config();
        config.setPath(file.name());
        config.load();

        auto data = config.data;

        auto data_color_start = data["start"];
        Color color_start = {
            data_color_start["red"],
            data_color_start["blue"],
            data_color_start["white"]};

        auto data_color_end = data["end"];
        Color color_end = {
            data_color_end["red"],
            data_color_end["blue"],
            data_color_end["white"]};

        Program(file.name(),
                data["start_time"].as<int>(),
                data["end_time"].as<int>(),
                color_start,
                color_end
                );

        file = root.openNextFile();
    }
}
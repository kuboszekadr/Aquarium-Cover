#include "Lighting.h"

void Lighting::loop()
{
    ESP32Time ts;
    uint32_t timestamp = ts.getTime("%H%M%S").toInt();

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
        uint32_t offset_in_minutes = secondToMin(offset);
        Program *program = getProgramToRun(timestamp, offset_in_minutes);

        uint32_t color = 0;
        if (program != nullptr)
        {
            color = program->getColor(timestamp, offset_in_minutes);
        }

        cover->setPixelColor(pixel, color);
        offset = offset + LIGHTING_PROGRAM_OFFSET;
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
        char file_name[16];
        
        extractFileName(file.name(), file_name);

        Config config = Config(file_name, root_path+1);
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

        new Program(file_name,
                data["start_time"].as<int>(),
                data["end_time"].as<int>(),
                color_start,
                color_end
                );

        file = root.openNextFile();
    }
}

uint32_t Lighting::secondToMin(uint32_t value)
{
    uint32_t result = (uint32_t) (value / 60) * 100;
    result += value % 60;
}

void extractFileName(const char* path, char* buff)
{
    char _path[33];
    memcpy(_path, path, 32);

    char *token;
    token = strtok(_path, "/");
    do
    {
        memcpy(buff, token, 15);
        token = strtok(NULL, "/");
    } while (token != NULL);

    memset(buff + strlen(buff) - 5, 0, 5);
}
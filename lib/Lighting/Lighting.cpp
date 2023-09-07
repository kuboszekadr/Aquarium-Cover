#include "Lighting.h"

cover_pixels Lighting::loop()
{
    time_t now;
    struct tm timestamp;

    time(&now);
    localtime_r(&now, &timestamp);

    Time t = Time(
        timestamp.tm_hour, 
        timestamp.tm_min, 
        timestamp.tm_sec
    );

    uint32_t epochs = t.epochs();
    return loop(epochs);

    // char time_str[11];
    // strftime(time_str, 10, "%H:%M:%S", &timestamp);

    // Serial.println(time_str);
}

cover_pixels Lighting::loop(uint32_t timestamp)
{
    std::vector<std::vector<uint32_t>> pixels;

    for (auto &cover : covers)
    {
        auto pixel = loopCover(cover, timestamp);
        pixels.push_back(pixel);
    }

    return pixels;
}

std::vector<uint32_t> Lighting::loopCover(Cover *cover, uint32_t timestamp)
{
    uint32_t offset = LIGHTING_PROGRAM_OFFSET * (cover->order() - 1);
    std::vector<uint32_t> pixels;

    for (uint32_t pixel = 0; pixel < cover->numPixels(); pixel++)
    {
        Program *program = getProgramToRun(timestamp, offset);
        uint32_t color = 0;
        
        if (program != nullptr)
        {
            color = program->getColor(timestamp, offset);
        }

        cover->setPixelColor(pixel, color);
        offset += LIGHTING_PROGRAM_OFFSET;

        pixels.push_back(color);
    }

    return pixels;
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

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

    uint32_t epochs = t.toMillis();
    return loopOverCovers(epochs);
}

/**
 * @brief Represents the pixels of the cover.
 */
cover_pixels Lighting::loopOverCovers(uint32_t timestamp)
{
    std::vector<std::vector<record>> pixels;

    for (auto &cover : covers)
    {
        auto pixel = loopOverCoverPixels(cover, timestamp);
        pixels.push_back(pixel);
    }

    return pixels;
}

/**
 * @brief Represents a vector of records.
 * 
 * A std::vector<record> is a container that holds a collection of records.
 * Each record is a tuple that contains color, offset, and program name.
 * 
 * @return The vector of records.
 */
std::vector<record> Lighting::loopOverCoverPixels(Cover *cover, uint32_t timestamp)
{
    uint32_t offset = LIGHTING_PROGRAM_OFFSET * (cover->order() - 1); // FIXME
    std::vector<record> pixels;

    for (uint32_t pixel = 0; pixel < cover->numPixels(); pixel++)
    {
        Program *program = getProgramToRun(timestamp, offset);
        uint32_t color = 0;
        std::string program_name("null");
        
        if (program != nullptr)
        {
            color = program->getColor(timestamp, offset);
            program_name.clear();
            program_name.append(program->name());
        }
        record _record = std::make_tuple(color, offset, program_name);

        cover->setPixelColor(pixel, color);
        offset += LIGHTING_PROGRAM_OFFSET;

        pixels.push_back(_record);
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

        Time start_time = Time(
            data["start_time"]["hour"],
            data["start_time"]["minute"],
            data["start_time"]["second"] | 0
        );

        Time end_time = Time(
            data["end_time"]["hour"],
            data["end_time"]["minute"],
            data["end_time"]["second"] | 0
        );
        
        new Program(file_name,
                start_time,
                end_time,
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

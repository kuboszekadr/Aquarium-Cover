#ifndef Lighting_h
#define Lighting_h

// that variable is relevant in that case because under the cover there are three
// lighting covers, unfortunatelly complicates the case a bit
#define LIGHTING_COVERS_AMOUNT 3
#define LIGHTING_PROGRAM_OFFSET 30L // offset in pixel program (in seconds)

#ifdef ARDUINO
    #include "Cover/Cover.h"
#else
    #include "Cover/CoverFake.h"
#endif

#include "Program/Program.h"
#include "Config/Config.h"

#include "Timestamp/Timestamp.h"
#include "Time.hpp"

#include <ArduinoJson.h>
#include <FS.h>
#include <vector>

typedef std::tuple<uint32_t, uint32_t, std::string> record;
typedef std::vector<std::vector<record>> cover_pixels;

namespace Lighting
{
    void setup();
    void begin();

    cover_pixels loop();
    cover_pixels loopOverCovers(uint32_t timestamp);

    void loadProgram(const char *file_name);
    std::vector<record> loopOverCoverPixels(Cover *cover, uint32_t timestamp);

    uint32_t secondToMin(uint32_t value);

    void runDemo(uint32_t duration);
}

void extractFileName(const char* path, char* buff);
#endif
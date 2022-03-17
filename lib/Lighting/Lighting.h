#ifndef Lighting_h
#define Lighting_h

// that variable is relevant in that case because under the cover there are three
// lighting covers, unfortunatelly complicates the case a bit
#define LIGHTING_COVERS_AMOUNT 3
#define LIGHTING_PROGRAM_OFFSET 30L // how each pixel row is turned ON (in seconds!)

#include "Cover/Cover.h"
#include "Program/Program.h"
#include "Config/Config.h"

#include <ArduinoJson.h>
#include <ESP32Time.h>
#include <FS.h>

namespace Lighting
{
    void setup();
    void begin();
    void loop();
    
    void loadProgram(const char *file_name);
    void loopCover(Cover *cover, uint32_t timestamp);

    uint32_t secondToMin(uint32_t value);

}

void extractFileName(const char* path, char* buff);

#endif
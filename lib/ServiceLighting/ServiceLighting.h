#ifndef ServiceLighting_h
#define ServiceLighting_h

#include "Services/Services.h"

#include "Program/Color.hpp"
#include "Program/Program.h"
#include "Cover/Cover.h"

#include "Lighting.h"
#include "Time.hpp"

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP32Time.h>
#include <dirent.h>
#include <sys/time.h>

namespace Services
{
    class ServiceLighting : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request);
        static void list_programs(AsyncWebServerRequest *request);
    };
}

#endif
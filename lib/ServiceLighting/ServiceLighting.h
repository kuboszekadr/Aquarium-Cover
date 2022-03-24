#ifndef ServiceLighting_h
#define ServiceLighting_h

#include "Services.h"

#include "../Lighting/Program/Program.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP32Time.h>

namespace Services
{
    class ServiceLighting : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request);
    };
}

#endif
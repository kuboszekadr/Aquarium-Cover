#ifndef ServiceConfig_h
#define ServiceConfig_h

#include "Services.h"

#include "Config/Config.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

namespace Services
{
    class ServiceConfig : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request, JsonVariant &json);
        static void post(AsyncWebServerRequest *request, JsonVariant &json);
    };
}

#endif
#ifndef ServiceWiFiConfig_h
#define ServiceWiFiConfig_h

#include "Services/Services.h"
#include "WiFiManager/WiFiManager.h"
#include "Config/Config.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

namespace Services
{
    class ServiceWiFi : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request);
        static void post(AsyncWebServerRequest *request, JsonVariant &json);
    };

    extern ServiceWiFi serviceWiFi;
}

#endif
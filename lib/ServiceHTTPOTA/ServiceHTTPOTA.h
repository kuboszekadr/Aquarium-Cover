#ifndef ServiceHTTPOTA_h
#define ServiceHTTPOTA_h

#include "Services/Services.h"
#include "WiFiManager/WiFiManager.h"
#include "Config/Config.h"

#include "soc/rtc_wdt.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <HTTPUpdate.h>

namespace Services
{
    class ServiceHTTPOTA : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request);
    };

    extern ServiceHTTPOTA service_http_ota;
}

#endif
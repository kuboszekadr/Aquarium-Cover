#ifndef ServiceConfig_h
#define ServiceConfig_h

#include "Services.h"

namespace Services
{
    class ServiceConfig : public IService
    {
    public:
        ServiceConfig(const char *uri);
        void create();

        static void get(AsyncWebServerRequest *request);
        static void post(AsyncWebServerRequest *request, JsonVariant &json);

    private:
        const char _uri[32] = "";
    };
}

#endif
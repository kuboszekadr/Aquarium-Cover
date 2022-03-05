#ifndef RelayService_h
#define RelayService_h

#include "../SmartHome-MicroContoler-Lib/Relay/Relay.h"
#include "Services.h"

namespace Services
{
    class ServiceRelay : public IService
    {
    public:
        ServiceRelay(const char *uri);
        void create();

        static void get(AsyncWebServerRequest * request);
        void post(AsyncWebServerRequest * request, JsonVariant & json);

    private:
        const char _uri[32] = "";
    };
}

#endif
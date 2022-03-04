#ifndef Services_h
#define Services_h

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include <vector>

namespace Services
{
    class IService
    {
    public:
        IService();
        virtual void create() = 0;
    };

    extern AsyncWebServer server;
    extern std::vector<IService*> services;

    void init();
}

#endif
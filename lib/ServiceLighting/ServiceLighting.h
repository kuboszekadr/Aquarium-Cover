#ifndef ServiceLighting_h
#define ServiceLighting_h

#include <esp_task_wdt.h>

#include "Services/Services.h"

#include "Program/Color.hpp"
#include "Program/Program.h"

#include "Cover/Cover.h"
#include "Demo/Demo.h"

#include "Timestamp/Timestamp.h"

#include "Lighting.h"
#include "Time.hpp"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <dirent.h>
#include <sys/time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>

namespace Services
{
    class ServiceLighting : public IService
    {
    public:
        void create();

        static void get(AsyncWebServerRequest *request);
        static void listPrograms(AsyncWebServerRequest *request);

        static void demoEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
        static void demoRequestHandler(void *arg, uint8_t *data, size_t len);

        static void calibrationEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
        static void calibrationRequestHandler(void *arg, uint8_t *data, size_t len);
    };
}

void runDemo(void *pvParameters);

void calibrationTaskStart(void *pvParameters);
void calibrationTaskStop(void *pvParameters);

extern Lighting::Demo *demo;

extern AsyncWebSocket demoSocket;
extern AsyncWebSocket configSocket;

extern TaskHandle_t DemoTaskHandler;
extern TaskHandle_t LightingTaskHandler;
extern TaskHandle_t CalibrationTaskHandler;

#endif
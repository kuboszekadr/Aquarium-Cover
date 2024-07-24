#include "Lighting.h"
#include "ServiceLighting.h"
#include "Programs/Programs.h"

#include "Demo/Demo.h"
#include "Device/Device.h"

#include "Services/Services.h"
#include "Services/ServiceSystemTime/ServiceSystemTime.h"
#include "Services/ServiceConfig/ServiceConfig.h"
#include "Services/ServiceOTA/ServiceOTA.h"
#include "Services/ServiceRestart/ServiceRestart.h"
#include "Services/ServiceSerial/ServiceSerial.h"

#include "ServiceHTTPOTA.h"
#include "ServiceWiFiConfig.h"

#include "Logger/Logger.h"
#include "Logger/Loggers/Serial.hpp"
#include "Logger/Loggers/API.hpp"
#include "Logger/Loggers/WebSerial.hpp"

#include "Notification/Notification.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

#include <HTTPUpdate.h>

#include <Adafruit_NeoPixel.h>

void setupTasks();

TaskHandle_t DemoTaskHandler;
TaskHandle_t LightingTaskHandler;
TaskHandle_t CalibrationTaskHandler;

Logger logger = Logger("main");

Lighting::Cover cover = Lighting::Cover(1, 19, 7);

// REST Endpoints
Services::ServiceSystemTime service_time = Services::ServiceSystemTime();
Services::ServiceOTA service_ota = Services::ServiceOTA();
Services::ServiceRestart service_restart = Services::ServiceRestart();

Services::ServiceLighting service_lighting = Services::ServiceLighting();
Services::ServiceConfig config_service = Services::ServiceConfig();
Services::ServiceWiFi wifi_service = Services::ServiceWiFi();
Services::ServiceHTTPOTA service_http_ota = Services::ServiceHTTPOTA();

// Sockets
AsyncWebSocket demoSocket("/s_demo");
AsyncWebSocket configSocket("/s_calibration");
AsyncWebSocket serial_socket("/s_serial");

Services::ServiceSerial serial_service = Services::ServiceSerial();

void GmailNotification(
    const char *title,
    const char *message);


void lightingTask(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    Logger::addStream(Loggers::logToSerial);
    Device::setup();

    Logger::addStream(Loggers::logToAPI);
    Logger::addStream(Loggers::logToWebSerial);

    esp_task_wdt_init(120, true);
    esp_task_wdt_add(NULL);

    setupTasks();
    Device::setupTime();

    auto lighthing_config = Config("device");
    lighthing_config.load();
    Lighting::pixel_offset = lighthing_config.data["pixel_offset"] | 0;

    Lighting::setup();
    Lighting::begin();

    Services::init();
    Services::serveHTMLFolder();

    Services::server.addHandler(&demoSocket);
    Services::server.addHandler(&configSocket);
    Services::server.addHandler(&serial_socket);

    Services::server.begin();

    logger.log("Device is starting...");

    Notification::addStream(GmailNotification);
    Notification::push("Cover-init", "Device started");

    xTaskCreate(
        lightingTask,
        "LightingMain",
        10000,
        NULL,
        1,
        &LightingTaskHandler);

    xTaskCreate(
        [](void *pvParameters)
        {
            for (;;)
            {
                Device::sendHeartbeat();
                vTaskDelay(30000 / portTICK_PERIOD_MS); // Delay for 30 seconds
            }
        },
        "HeartbeatTask", // Name of task
        10000,           // Stack size of task
        NULL,            // Parameter of the task
        1,               // Priority of the task
        NULL             // Task handle to keep track of created task
    );

    xTaskCreate(
        [](void *pvParameters)
        {
            for (;;)
            {
                WiFiManager::manageConnection();
                vTaskDelay(60000 / portTICK_PERIOD_MS); // Delay for 60 seconds
            }
        },
        "WiFiConnection",
        10000,
        NULL,
        1,
        NULL);

    // checkForUpdates();
}

void loop()
{
    demoSocket.cleanupClients();
    configSocket.cleanupClients();
    serial_socket.cleanupClients();
    
    Cron.delay();

    esp_task_wdt_reset();
}

void lightingTask(void *pvParameters)
{
    for (;;)
    {
        (void)Lighting::loop();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setupTasks()
{
    Cron.create(
        "0 0 4 * * *",
        Device::setupTime,
        false);
}

void GmailNotification(const char *title, const char *message)
{
    Device::device->postNotification(title, message);
}


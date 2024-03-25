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

#include "ServiceHTTPOTA.h"
#include "ServiceWiFiConfig.h"

#include "Logger/Logger.h"
#include "Logger/Loggers/Serial.hpp"
#include "Logger/Loggers/API.hpp"

#include "Notification/Notification.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

#include <HTTPUpdate.h>

void setupTasks();

TaskHandle_t DemoTaskHandler;
TaskHandle_t LightingTaskHandler;
TaskHandle_t CalibrationTaskHandler;

AsyncWebSocket demoSocket("/s_demo");
AsyncWebSocket configSocket("/s_calibration");

Logger logger = Logger("main");

Lighting::Cover cover = Lighting::Cover(1, 19, 10);

Services::ServiceSystemTime service_time = Services::ServiceSystemTime();
Services::ServiceOTA service_ota = Services::ServiceOTA();
Services::ServiceRestart service_restart = Services::ServiceRestart();

Services::ServiceLighting service_lighting = Services::ServiceLighting();
Services::ServiceConfig config_service = Services::ServiceConfig();
Services::ServiceWiFi wifi_service = Services::ServiceWiFi();
Services::ServiceHTTPOTA service_http_ota = Services::ServiceHTTPOTA();

void GmailNotification(
    const char *title,
    const char *message);

void checkForUpdates();
bool updateFirmware();
bool updateFilesystem();

void setup()
{
    Serial.begin(115200);

    Logger::addStream(Loggers::logToSerial);
    Device::setup();
    
    Serial.println("v0.1.1");

    // Logger::addStream(Loggers::logToAPI);

	esp_task_wdt_init(120, true);
	esp_task_wdt_add(NULL);

    setupTasks();
    Device::setupTime();

    Lighting::setup();
    Lighting::begin();

    Services::init();

    Services::server.on(
        "/wifi_config", 
        HTTP_GET, 
        [](AsyncWebServerRequest *request)
        {
            request->send(
                SPIFFS, 
                "/html/wifi.html", 
                "text/html");
        }
    );

    Services::server.on(
        "/demo", 
        HTTP_GET, 
        [](AsyncWebServerRequest *request)
        {
            request->send(
                SPIFFS, 
                "/html/demo.html", 
                "text/html");
        }
    );

    Services::server.on(
        "/tests", 
        HTTP_GET, 
        [](AsyncWebServerRequest *request)
        {
            request->send(
                SPIFFS, 
                "/html/tests.html", 
                "text/html");
        }
    );

    Services::server.on(
        "/calibration", 
        HTTP_GET, 
        [](AsyncWebServerRequest *request)
        {
            request->send(
                SPIFFS, 
                "/html/calibration.html", 
                "text/html");
        }
    );
    
    Services::server.addHandler(&demoSocket);
    Services::server.addHandler(&configSocket);

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

    checkForUpdates();
}

void loop()
{
    demoSocket.cleanupClients();
    Cron.delay();

    esp_task_wdt_reset();
}

// void lightingTask(void *pvParameters)
// {
//     for (;;)
//     {
//         Lighting::loop();
//         vTaskDelay(500 / portTICK_PERIOD_MS);
//     }
// }

void setupTasks()
{
    Cron.create(
        "0 0 4 * * *",
        Device::setupTime,
        false);

    Cron.create(
        "* */1 * * * *",
        WiFiManager::manageConnection,
        false);

    // Cron.create(
    //     "*/5 * * * * *",
    //     [] () {Lighting::loop();},
    //     false);

	Cron.create(
		"*/30 * * * * *",
		Device::sendHeartbeat,
		false);        
}

void GmailNotification(const char *title, const char *message)
{
    Device::device->postNotification(title, message);
}

void checkForUpdates()
{
    HTTPClient client;
    client.begin("http://74.234.8.4:5000/updates");
    client.setConnectTimeout(1000);

    int response_code = client.GET();
    if (response_code != 200)
    {
        logger.log("Could not check...");
    }

    JsonDocument doc;

    deserializeJson(doc, client.getString());
    serializeJsonPretty(doc, Serial);
    
    client.end();

    bool update_firmware = bool(doc["firmware"] == 1 | 0);
    bool update_filesystem = bool(doc["filesystem"] == 1 | 0);
    bool updated = false;

    httpUpdate.rebootOnUpdate(false);
    if (update_firmware)
    {
        delay(1000);
        updated = updateFirmware();
    }
    
    if (update_filesystem)
    {
        delay(1000);
        updated = (updated | updateFilesystem());
    }

    if (updated)
    {
        Serial.println("Filesystem or firmware has been updated. Restarting.");
        ESP.restart();
    }

}

bool updateFirmware()
{
    Serial.println("Updating firmware...");

    WiFiClient client;

    const char* update_url = "http://74.234.8.4:5000/download_latest";
    t_httpUpdate_return ret = httpUpdate.update(client, update_url);

    bool result = false;

    switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf(
                "HTTP_UPDATE_FAILED Error (%d): %s\n", 
                httpUpdate.getLastError(), 
                httpUpdate.getLastErrorString().c_str()
            );
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");

            break;

        case HTTP_UPDATE_OK:
            result = true;
            Serial.println("HTTP_UPDATE_OK");

            break;
    }

    return result;
}

bool updateFilesystem()
{
    Serial.println("Updating filesystem image...");

    WiFiClient client;
    const char* update_url = "http://74.234.8.4:5000/download_latest_fs";
    t_httpUpdate_return ret = httpUpdate.updateSpiffs(client, update_url);
    bool result = false;

    switch (ret) {
    case HTTP_UPDATE_FAILED:
        Serial.printf(
            "HTTP_UPDATE_FAILD Error (%d): %s\n", 
            httpUpdate.getLastError(), 
            httpUpdate.getLastErrorString().c_str()
        );
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        result = true;
        Serial.println("HTTP_UPDATE_OK");
        break;
    }        

    return result;

}
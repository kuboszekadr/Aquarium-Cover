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

Lighting::Cover cover = Lighting::Cover(1, 19, 10);

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

void checkForUpdates();
void increaseBright();

bool updateFirmware();
bool updateFilesystem();

uint8_t brightness = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10, 19, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);

    Logger::addStream(Loggers::logToSerial);
    Device::setup();
    Device::device->setupAPI(false);

    Serial.println("v0.0.1");

    // Logger::addStream(Loggers::logToAPI);
    Logger::addStream(Loggers::logToWebSerial);

	esp_task_wdt_init(120, true);
	esp_task_wdt_add(NULL);

    setupTasks();
    Device::setupTime();

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

    Cron.create(
        "*/10 * * * * *",
        increaseBright,
        false
    );
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

void increaseBright()
{
    logger.logf("%d", brightness);
    
    for (uint8_t i = 0; i < 10; i++)
    {
        pixels.setPixelColor(i, brightness, brightness, brightness);
    }
    pixels.show();    

    brightness++;
}
#include "Lighting.h"
#include "ServiceLighting.h"
#include "Programs/Programs.h"

#include "Demo/Demo.h"
#include "Device/Device.h"

#include "Services/Services.h"
#include "Services/ServiceSystemTime/ServiceSystemTime.h"
#include "Services/ServiceConfig/ServiceConfig.h"
#include "Services/ServiceOTA/ServiceOTA.h"

#include "Logger/Logger.h"
#include "Logger/Loggers/Serial.hpp"
#include "Logger/Loggers/API.hpp"

#include "Notification/Notification.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

void setupTasks();

Logger logger = Logger("main");

Lighting::Cover left_cover = Lighting::Cover(1, 12, 10);
Lighting::Cover center_cover = Lighting::Cover(2, 13, 10);
Lighting::Cover right_cover = Lighting::Cover(3, 15, 10);

Services::ServiceSystemTime service_time = Services::ServiceSystemTime();
Services::ServiceOTA service_ota = Services::ServiceOTA();
Services::ServiceLighting service_lighting = Services::ServiceLighting();
Services::ServiceConfig config_service = Services::ServiceConfig();

void GmailNotification(
    const char *title,
    const char *message);

void setup()
{
    Serial.begin(115200);

    Logger::addStream(Loggers::logToSerial);
    Device::setup();
    
    // Logger::addStream(Loggers::logToAPI);

    setupTasks();
    Device::setupTime();

    Lighting::setup();
    Lighting::begin();

    Services::init();
    Services::server.begin();

    logger.log("Device is starting...");

    Notification::addStream(GmailNotification);
    Notification::push("Cover-init", "Device started");
}

void loop()
{
    Cron.delay();
}

void setupTasks()
{
    char task[30] = "0 0 4 * * *";
    Cron.create(
        task,
        Device::setupTime,
        false);

    Cron.create(
        "* */1 * * * *",
        WiFiManager::manageConnection,
        false);

    Cron.create(
        "*/5 * * * * *",
        [] () {Lighting::loop();},
        false);
}

void GmailNotification(const char *title, const char *message)
{
    Device::device->postNotification(title, message);
}
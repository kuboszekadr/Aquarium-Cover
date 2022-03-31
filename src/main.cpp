#include "Lighting.h"
#include "ServiceLighting.h"

#include "Services/Services.h"
#include "Services/ServiceSystemTime/ServiceSystemTime.h"
#include "Services/ServiceConfig/ServiceConfig.h"
#include "Services/ServiceOTA/ServiceOTA.h"

#include "Device/Device.h"
#include "Logger/Logger.h"
#include "Notification/Notification.h"
#include "Logger/Streams/StreamSerial.hpp"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

void setupTasks();

Lighting::Cover left_cover = Lighting::Cover(1, 1, 6);
// Lighting::Cover middle_cover = Lighting::Cover(2, 2, 8);
// Lighting::Cover right_cover = Lighting::Cover(3, 3, 6);

void setup()
{
    Serial.begin(115200);
    Logger::addStream(Streams::streamToSerial);

    Device::setup();

    setupTasks();
    Lighting::setup();
    Lighting::begin();

    Services::init();
    Services::server.begin();

    Notification::push("Cover-init", "Device started");
}

void loop()
{
    Cron.delay();
    Lighting::loop();
    WiFiManager::manageConnection();
}

void setupTasks()
{
    char task[30] = "0 0 4 * * *";
    Cron.create(
        task,
        Device::setupTime,
        false);
}

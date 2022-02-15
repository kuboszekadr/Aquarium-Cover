#include "Lighting.h"
#include "Device/Device.h"
#include "Logger/Logger.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

void setupTasks();

void setup()
{
    setupTasks();
    Lighting::begin();
}

void loop()
{
    Cron.delay();
    Lighting::loop();
    WiFiManager::manageConnection();
}

void setupTasks()
{
    Cron.create(
        "0 0 4 * * *",
        Device::setupTime,
        false);
}
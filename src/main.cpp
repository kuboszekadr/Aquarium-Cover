#include "Lighting.h"
#include "Services.h"
#include "ServiceSystemTime.h"
#include "ServiceConfig.h"

#include "Device/Device.h"
#include "Logger/Logger.h"
#include "Notification/Notification.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

void setupTasks();
void streamToSerial(const char *module_name,
                    const char *log_level,
                    const char *msg,
                    const char *timestamp);

Services::ServiceSystemTime time_service = Services::ServiceSystemTime();
Services::ServiceConfig config_service = Services::ServiceConfig();

Lighting::Cover left_cover = Lighting::Cover(1, 1, 6);
Lighting::Cover middle_cover = Lighting::Cover(2, 2, 8);
Lighting::Cover right_cover = Lighting::Cover(3, 3, 6);

void setup()
{
    Serial.begin(115200);
    Logger::addStream(streamToSerial);

    Device::setupSPIFSS();
    Device::setupWiFi();
    Device::setupAPI();
    Device::setupTime();

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

void streamToSerial(const char *module_name,
                    const char *log_level,
                    const char *msg,
                    const char *timestamp)
{
  char _msg[256];
  sprintf(_msg,
          "%s | %s | [%s] %s",
          module_name, log_level, timestamp, msg);
  Serial.println(_msg);
}
#include "Lighting.h"
#include "Services.h"
#include "ServiceSystemTime.h"

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

void setup()
{
    Serial.begin(115200);
    Logger::addStream(streamToSerial);

    Device::setupSPIFSS();
    Device::setupWiFi();
    Device::setupAPI();
    Device::setupTime();

    setupTasks();
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
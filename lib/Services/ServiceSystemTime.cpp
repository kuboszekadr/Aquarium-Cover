#include "ServiceSystemTime.h"

void Services::ServiceSystemTime::create()
{
    server.on("/time", HTTP_GET, get);
}

void Services::ServiceSystemTime::get(AsyncWebServerRequest *request)
{
    ESP32Time t;
    String system_time = t.getTime("%F %X");

    StaticJsonDocument<80> doc;
    JsonObject result = doc.to<JsonObject>();
    result["time"] = system_time;

    char response[80];
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}


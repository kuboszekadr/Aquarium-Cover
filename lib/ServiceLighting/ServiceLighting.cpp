#include "ServiceLighting.h"

void Services::ServiceLighting::create()
{
    server.on("/lighting", HTTP_GET, get);
}

void Services::ServiceLighting::get(AsyncWebServerRequest *request)
{
    ESP32Time ts;
    uint32_t timestamp = ts.getTime("%H%M%S").toInt();

    float progress = 0.0;
    uint32_t color = 0;
    char name[16] = "None";

    Lighting::Program *current_program = Lighting::getProgramToRun(timestamp);
    if (current_program != nullptr)
    {
        progress = current_program->progress(timestamp);
        color = current_program->getColor(timestamp);
        memcpy(name, current_program->name(), 16);
    }

    StaticJsonDocument<512> doc;

    doc["current_program"] = name;
    doc["progress"] = progress;
    doc["color"] = color;
    doc["timestamp"] = timestamp;

    String response;
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}

Services::ServiceLighting lighting_service = Services::ServiceLighting();

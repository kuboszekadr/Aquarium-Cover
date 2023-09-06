#include "ServiceLighting.h"

void Services::ServiceLighting::create()
{
    server.on("/lighting", HTTP_GET, get);
    server.on("/list_programs", HTTP_GET, list_programs);
}

void Services::ServiceLighting::get(AsyncWebServerRequest *request)
{
    time_t now;
    struct tm timestamp;

    time(&now);
    localtime_r(&now, &timestamp);

    uint32_t ts = Time(timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec).epochs();

    float progress = 0.0;
    uint32_t color = 0;
    
    char name[16] = "null";
    memset(name, 0, 15);

    Lighting::Program *current_program = Lighting::getProgramToRun(ts);
    if (current_program != nullptr)
    {
        progress = current_program->progress(ts);
        color = current_program->getColor(ts);
        std::memcpy(name, current_program->name(), 15);
    }

    StaticJsonDocument<512> doc;

    doc["current_program"] = name;
    doc["progress"] = progress;

    char time_str[11];
    strftime(time_str, 10, "%H:%M:%S", &timestamp);
    doc["timestamp"] = time_str;

    JsonObject doc_color = doc.createNestedObject("color");
    doc_color["blue"] = (color >> 16) & 0xFF;
    doc_color["red"] = (color >> 8) & 0xFF;;
    doc_color["white"] = color & 0xFF;

    String response;
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}

void Services::ServiceLighting::list_programs(AsyncWebServerRequest *request)
{
    const char root_path[32] = "/config/lighting";

    File root = SPIFFS.open(root_path);
    File file = root.openNextFile();
    
    StaticJsonDocument<256> doc;
    JsonArray result = doc.to<JsonArray>();

    while (file)
    {
        char file_name[16];

        extractFileName(file.name(), file_name); 
        result.add(file_name);
        file = root.openNextFile();       
    }

    String response;
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response
    );
}

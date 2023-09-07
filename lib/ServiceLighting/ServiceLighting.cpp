#include "ServiceLighting.h"

void Services::ServiceLighting::create()
{
    server.on("/lighting", HTTP_GET, get);
    server.on("/list_programs", HTTP_GET, list_programs);
}

void Services::ServiceLighting::get(AsyncWebServerRequest *request)
{

    auto pixels  = Lighting::loop();

    DynamicJsonDocument doc(3000);
    JsonArray array = doc.to<JsonArray>();

    for (const auto &cover : pixels)
    {   
        JsonArray nested_array = array.createNestedArray();
        for (const auto &pixel : cover)
        {
            auto pixel_data = nested_array.createNestedObject();
            Lighting::Color rgb = Lighting::Color::fromPixelColor(pixel);

            pixel_data["red"] = rgb.red;
            pixel_data["blue"] = rgb.blue;
            pixel_data["white"] = rgb.white;
        }
    }

    time_t now;
    struct tm timestamp;

    time(&now);
    localtime_r(&now, &timestamp);

    char time_str[11];
    strftime(time_str, 10, "%H:%M:%S", &timestamp);
    doc["timestamp"] = time_str;

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

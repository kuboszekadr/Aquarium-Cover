#include "ServiceLighting.h"

void Services::ServiceLighting::create()
{
    server.on("/lighting", HTTP_GET, get);
    server.on("/list_programs", HTTP_GET, list_programs);
}

void Services::ServiceLighting::get(AsyncWebServerRequest *request)
{
    auto pixels  = Lighting::loop();

    JsonDocument doc;
    auto data = doc.to<JsonArray>();

    for (const auto &cover : pixels)
    {   
        auto cover_data = data.createNestedObject();
        cover_data["cover_nb"] = 1;

        auto cover_pixels = cover_data.createNestedArray("pixels");
        for (const auto &pixel : cover)
        {
            auto pixel_data = cover_pixels.createNestedObject();
            uint32_t color = std::get<0>(pixel);
            
            Lighting::Color rgb = Lighting::Color::fromPixelColor(color);

            auto pixel_color = pixel_data.createNestedObject("pixel");
            pixel_color["red"] = rgb.red;
            pixel_color["blue"] = rgb.blue;
            pixel_color["white"] = rgb.white;

            pixel_data["offset"] = std::get<1>(pixel);
            pixel_data["program"] = std::get<2>(pixel);
        }
    }

    char time_str[11];
    strcpy(time_str, getTimestamp());
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

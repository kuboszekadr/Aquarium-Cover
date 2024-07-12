#include "ServiceLighting.h"

Lighting::Demo *demo = nullptr;

void Services::ServiceLighting::create()
{
    server.on("/lighting", HTTP_GET, get);
    server.on("/list_programs", HTTP_GET, listPrograms);
    server.on("/testing", HTTP_GET, get);

    demoSocket.onEvent(demoEvent);
    configSocket.onEvent(calibrationEvent);
}

void Services::ServiceLighting::get(AsyncWebServerRequest *request)
{
    auto pixels = Lighting::loop();

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

void Services::ServiceLighting::listPrograms(AsyncWebServerRequest *request)
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
        response);
}

void Services::ServiceLighting::demoEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        break;
    case WS_EVT_DISCONNECT:
        break;
    case WS_EVT_DATA:
        demoRequestHandler(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void Services::ServiceLighting::demoRequestHandler(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (
        info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        if (eTaskGetState(LightingTaskHandler) == eSuspended)
        {
            demoSocket.textAll("Demo already running");
            return;
        }

        JsonDocument obj;
        DeserializationError err = deserializeJson(obj, data);

        JsonObject start = obj["start"];
        JsonObject end = obj["end"];

        demo = new Lighting::Demo(
            Time(
                start["hour"],
                start["minute"],
                start["seconds"]),
            Time(
                end["hour"],
                end["minute"],
                end["seconds"]),
            obj["duration"].as<uint32_t>());

        xTaskCreate(
            runDemo, // Add class scope resolution operator
            "runDemo",
            20000,
            demo,
            1,
            NULL);
    }
}

void Services::ServiceLighting::calibrationEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        vTaskSuspend(LightingTaskHandler);
        break;
    case WS_EVT_DISCONNECT:
        Serial.println("Client disconected");
        Serial.println("Resuming lighting task");
        vTaskResume(LightingTaskHandler);
        break;
    case WS_EVT_DATA:
        calibrationRequestHandler(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void Services::ServiceLighting::calibrationRequestHandler(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (
        info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        JsonDocument obj;
        DeserializationError err = deserializeJson(obj, data);

        uint8_t red = obj["red"].as<uint8_t>();
        uint8_t blue = obj["blue"].as<uint8_t>();
        uint8_t white = obj["white"].as<uint8_t>();

        Lighting::Color color = Lighting::Color{red, blue, white};

        for (auto &cover : Lighting::covers)
        {
            cover->begin();
            for (uint32_t pixel = 0; pixel < cover->numPixels(); pixel++)
            {
                cover->setPixelColor(pixel, color.toPixelColor());
            }
            cover->show();
        }
    }
}

void runDemo(void *pvParameters)
{
    vTaskSuspend(LightingTaskHandler);
    Lighting::Demo *demo = (Lighting::Demo *)pvParameters;

    uint32_t refresh_rate = 1 * 500; // in miliseconds
    uint32_t last_client_refresh = millis();

    uint32_t ts = demo->_start.toMillis();
    uint32_t end_ts = demo->_end.toMillis();

    while (ts <= end_ts)
    {
        if ((millis() - last_client_refresh) >= refresh_rate)
        {
            Time _ts = Time(ts);

            JsonDocument doc;

            JsonObject time = doc["time"].to<JsonObject>();

            time["hour"] = _ts.hour;
            time["minute"] = _ts.minute;
            time["seconds"] = _ts.seconds;

            JsonObject pixel = doc["pixel"].createNestedObject();
            pixel["offset"] = std::get<1>(demo->pixel);
            pixel["program"] = std::get<2>(demo->pixel);

            Lighting::Color color = Lighting::Color::fromPixelColor(std::get<0>(demo->pixel));
            pixel["red"] = color.red;
            pixel["blue"] = color.blue;
            pixel["white"] = color.white;

            String msg;
            serializeJson(doc, msg);

            demoSocket.textAll(msg);
            last_client_refresh = millis();
            ts = demo->runStep(ts);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    demoSocket.closeAll();

    delete demo;
    demo = nullptr;

    vTaskResume(LightingTaskHandler);
    vTaskDelete(NULL);
}
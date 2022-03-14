#include "ServiceConfig.h"

void Services::ServiceConfig::create()
{
    server.on("/config", HTTP_GET, get);

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
        "/config",
        post);
    handler->setMethod(HTTP_POST);
    server.addHandler(handler);
}

void Services::ServiceConfig::get(AsyncWebServerRequest *request)
{
    auto remote_ip = request->client()->remoteIP();

    const char *arg = request->argName(0).c_str();
    Config config = Config(arg);
    config.load();

    char response[256] = "";
    serializeJson(config.data, response);

    request->send(
        200,
        "application/json",
        response);
}

void Services::ServiceConfig::post(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject obj = json.as<JsonObject>();

    const char *arg = request->argName(0).c_str();
    Config config = Config(arg);
    config.load();

    config.data = obj;

    int status = config.save();
    if (status != CONFIG_SAVED)
    {
        request->send(500);
    }
    request->send(200);
}
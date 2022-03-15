#include "ServiceConfig.h"

void Services::ServiceConfig::create()
{
    AsyncCallbackJsonWebHandler *get_handler = new AsyncCallbackJsonWebHandler(
        "/config",
        get);
    get_handler->setMethod(HTTP_GET);
    server.addHandler(get_handler);

    AsyncCallbackJsonWebHandler *post_handler = new AsyncCallbackJsonWebHandler(
        "/config",
        post);
    post_handler->setMethod(HTTP_POST);
    server.addHandler(post_handler);
}

void Services::ServiceConfig::get(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject obj = json.as<JsonObject>();
    
    const char* file = obj["file_name"];
    const char* folder = obj["folder"] | "config";

    Config config = Config(file, folder);
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

    const char* file = obj["file_name"];
    const char* folder = obj["folder"] | "config";
    
    StaticJsonDocument<512> doc;
    deserializeJson(doc, obj["content"]);

    Config config = Config(file, folder);
    config.load();

    config.data = doc;

    int status = config.save();
    if (status != CONFIG_SAVED)
    {
        request->send(500);
    }
    request->send(200);
}
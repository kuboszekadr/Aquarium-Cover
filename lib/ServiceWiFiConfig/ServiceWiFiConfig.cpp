#include "ServiceWiFiConfig.h"

void Services::ServiceWiFi::create()
{
    server.on("/wifi", HTTP_GET, get);

    AsyncCallbackJsonWebHandler *post_handler = new AsyncCallbackJsonWebHandler(
        "/wifi",
        post);
    post_handler->setMethod(HTTP_POST);
    server.addHandler(post_handler);
}

void Services::ServiceWiFi::get(AsyncWebServerRequest *request)
{
    Config config = Config("wifi");
    config.load();

    String response;
    serializeJson(config.data, response);

    request->send(
        200,
        "application/json",
        response
        );
}

void Services::ServiceWiFi::post(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject obj = json.as<JsonObject>();

    String request_str;
    serializeJsonPretty(obj, request_str);
    Serial.println(request_str);

    const char *ssid = obj["ssid"];
    const char *pwd = obj["pwd"];

    Serial.println("Test");

    wl_status_t status = WiFiManager::connect(ssid, pwd);

    if (status != WL_CONNECTED)
    {
        request->send(500);
        return;
    }

    request->send(200);
}
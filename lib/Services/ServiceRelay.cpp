#include "ServiceRelay.h"

void Services::ServiceRelay::create()
{
    server.on(_uri, HTTP_GET, get);
}

void Services::ServiceRelay::get(AsyncWebServerRequest *request)
{
    StaticJsonDocument<200> doc;
    JsonArray result = doc.to<JsonArray>();

    for (const auto &relay : Relay::relays)
    {
        JsonObject pin = result.createNestedObject();
        pin["pin"] = relay.first;
        pin["status"] = relay.second->getState();
    }

    char response[256];
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}

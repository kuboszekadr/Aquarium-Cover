#include "ServiceHTTPOTA.h"

void Services::ServiceHTTPOTA::create()
{
    server.on("/http_ota", HTTP_GET, get);
}

void Services::ServiceHTTPOTA::get(AsyncWebServerRequest *request)
{
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;

    rtc_wdt_protect_off();
    rtc_wdt_disable();
    const char* update_url = "http://74.234.8.4:5000/download_bin";
    // const char* update_url = "http://192.168.0.107:5000/download_bin";

    WiFiClient client;

    t_httpUpdate_return ret = httpUpdate.update(client, update_url);

    switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf(
                "HTTP_UPDATE_FAILED Error (%d): %s\n", 
                httpUpdate.getLastError(), 
                httpUpdate.getLastErrorString().c_str()
            );
            

            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");

            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");

            break;
    }

}

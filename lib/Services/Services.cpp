#include "Services.h"

AsyncWebServer Services::server = AsyncWebServer(80);
std::vector<Services::IService*> Services::services = std::vector<Services::IService *>();

Services::IService::IService()
{
    services.push_back(this);
}

void Services::init()
{
    for(const auto& service : services)
    {
        service->create();
    }
}

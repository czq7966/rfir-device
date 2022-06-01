
#ifndef __NETWORK_MODULE_OTA_UPDATER_H__
#define __NETWORK_MODULE_OTA_UPDATER_H__

#include <string>
#include "EOTAUpdate.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

namespace network {
    namespace module {
        namespace ota {
            class Updater {
            private:
                EOTAUpdate* eOtaUpdater = 0;                
                WiFiEventHandler wifiConnectHandler;
                WiFiEventHandler wifiDisconnectHandler;
                uint32_t m_update_handler = 0;
            public:
                struct Params {
                    std::string url;
                    std::string id;
                    unsigned version;
                    uint32_t interval = 1000 * 60 * 60; //1小时
                };            
            public:
                Params params;
                void start(Params p);
                void loop();
            public:
                void onWifiConnect(const WiFiEventStationModeGotIP& event);
                void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);    
            public:
                void  checkAndUpdate();
                void* doCheckAndUpdate(void* arg, void* p);

            };

        }
    }
}

extern network::module::ota::Updater GOTAUpdater;

#endif //

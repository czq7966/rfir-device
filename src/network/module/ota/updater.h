
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
                uint32_t m_update_handler = 0;
            public:
                struct Params {
                    const char* url;
                    const char* id;
                    unsigned version;
                    bool     enabled = true;
                    bool     reboot = false;
                    uint32_t interval = 60 * 24; //1天
                };            
            public:
                Params params;
                void start();
                void loop();
            public:
                void* onWifiConnect(void* arg, void* p);
                void* onWifiDisconnect(void* arg, void* p); 
            public:
                void  checkAndUpdate();
                void* doCheckAndUpdate(void* arg, void* p);

            };

        }
    }
}

extern network::module::ota::Updater GOTAUpdater;

#endif //

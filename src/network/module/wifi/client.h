#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include <vector>
#include "ap.h"
#include "smc.h"

namespace network {
    namespace module {
        namespace wifi {
            class Client {
            public:
                struct Params {
                    bool                apMode = false;
                    bool                smcMode= false;
                    std::vector<std::string>    ssid;
                    std::vector<std::string>    pass;                   
                    // std::vector<int>            timeout;
                    uint32_t                    timeout;
                    AP::Params          ap;
                    SMC::Params         smc;
                    bool                assign(Params& p);
                };
            public:
                Params  params;
            public:
                void start(Params& p);
                void loop();
                void checkOrReset();
                void multiCheckOrReset();
                bool connectWifi(std::string ssid, std::string pass, int timeout);
                int  connectWifi();
                void multiCheckOrReset2();

            //V2
            public:
                int m_connect_ssid_index = 0;
                uint32_t m_connect_timeout_handler = 0;
                JLed* led = 0; // = WIFI_CONNECT_JLED;
                WiFiEventHandler wifiConnectHandler;
                WiFiEventHandler wifiDisconnectHandler;

                void  startV2();
                void  loopV2();

                void  checkLed();
                void  connectToWifi();
                void  delayConnectToWifi();
                void* doConnectToWifi(void* arg, void* p);
            public:
                void onWifiConnect(const WiFiEventStationModeGotIP& event);
                void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
                void* onWifiConnectTimeout(void* arg, void* p);

            };
        }
    }
}

extern network::module::wifi::Client GWifiClient;

#endif
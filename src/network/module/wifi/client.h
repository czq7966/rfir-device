#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include <vector>
#include "ap.h"
#include "smc.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

namespace network {
    namespace module {
        namespace wifi {
            class Client {
            public:
                struct Events {
                    rfir::util::Event onWifiConnect;
                    rfir::util::Event onWifiDisconnect;
                };
                struct Params {
                    bool                apMode = false;
                    bool                smcMode= false;
                    std::vector<std::string>    ssid;
                    std::vector<std::string>    pass;                   
                    uint32_t                    timeout;
                    uint32_t                    interval = 30 * 1000;
                    AP::Params          ap;
                    SMC::Params         smc;
                    bool                assign(Params& p);
                };
            public:
                Events  events;
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
                uint32_t m_check_timeout_handler = 0;

                void  startV2();
                void  loopV2();

                void  connectToWifi();
                void  delayConnectToWifi();
                void* doConnectToWifi(void* arg, void* p);
            public:
#ifdef ESP8266               
                WiFiEventHandler wifiConnectHandler;
                WiFiEventHandler wifiDisconnectHandler;

                void _onWifiConnect(const WiFiEventStationModeGotIP& event);
                void _onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
#else
                void WiFiEvent(WiFiEvent_t event);
#endif                
                void* onWifiConnect(void* arg, void* p);
                void* onWifiDisconnect(void* arg, void* p);
                void* onWifiConnectTimeout(void* arg, void* p);
                void* onWifiCheckTimeout(void* arg, void* p);


            };
        }
    }
}

extern network::module::wifi::Client GWifiClient;

#endif
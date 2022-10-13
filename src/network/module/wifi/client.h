#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include <vector>
#include "rfir/util/led.h"
#include "rfir/util/event-emitter.h"

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
                    rfir::util::Event softAPModeStationConnected;
                    rfir::util::Event softAPModeStationDisconnected;
                };
                struct Params {
                    std::vector<const char*>    ssid;
                    std::vector<const char*>    pass;                   
                    uint32_t                    timeout;
                    uint32_t                    interval = 30 * 1000;
                    JLed*                       jled = 0;
                };
            public:
                Events  events;
                Params  params;
            public:
                void start();
                void loop();
            //V2
            public:
                int m_connect_ssid_index = 0;
                uint32_t m_connect_timeout_handler = 0;
                uint32_t m_check_timeout_handler = 0;
                uint32_t m_delay_connect_handler = 0;

                void  startV2();
                void  loopV2();

                void  connectToWifi();
                void  connectToNextWifi();
                void  removeDelayConnectHandler();
                void  removeConnectTimeoutHandler();
                void  delayConnectToWifi(int timeoutms = 15000);
                void  delayConnectToNextWifi(int timeoutms = 15000);
                void* doConnectToWifi(void* arg, void* p);
                void* doConnectToNextWifi(void* arg, void* p);
            public:
#ifdef ESP8266               
                WiFiEventHandler wifiConnectHandler;
                WiFiEventHandler wifiDisconnectHandler;
                WiFiEventHandler softAPModeStationConnectedHandler;
                WiFiEventHandler softAPModeStationDisconnectedHandler;

                void _onWifiConnect(const WiFiEventStationModeGotIP& event);
                void _onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
                void _onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& event);
                void _onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& event);
#else
                void WiFiEvent(WiFiEvent_t event);
#endif                
                void* onWifiConnect(void* arg, void* p);
                void* onWifiDisconnect(void* arg, void* p);
                void* onWifiConnectTimeout(void* arg, void* p);
                void* onWifiCheckTimeout(void* arg, void* p);
            public:
                void clearSsids();
                void addSsids(const char* ssid, const char* pass);
                void addSsid(const char* ssid, const char* pass);
            };
        }
    }
}

extern network::module::wifi::Client GWifiClient;

#endif
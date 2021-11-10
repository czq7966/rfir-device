#ifndef __NETWORK_MODULE_WIFI_AP_H__
#define __NETWORK_MODULE_WIFI_AP_H__

#include <string>
#include <IotWebConf.h>

namespace network {
    namespace module {
        namespace wifi {
            class AP {
            public:
                struct Params {
                    std::string apSsid;
                    std::string apPass;
                    std::string wifiSsid;
                    std::string wifiPass;
                    std::string configVersion = "0";
                    int         configPin = -1;
                    int         configPinTimeout = 5;
                    long        resetTimeout = 60 * 5;
                    int         statusPin = LED_BUILTIN;
#ifdef ESP32                    
                    int         statusPinOnLevel = HIGH;
#else
                    int         statusPinOnLevel = LOW;
#endif                    
                };
            protected:
                long reset_timeout_start = 0;
                void handleRoot();                
            public:
                DNSServer* dnsServer;
                WebServer* webServer;
                IotWebConf* iotWebConf;
                Params params;
            public:
                void start(Params p);
                void loop();
                void checkOrReset();
                void applyDefault();
                void setupConfigPin();
                void checkConfigPin();
            public:
                static long ConfigPinChangeTime;
                static long ConfigPinChangeTime_Low;
                static long ConfigPinChangeTime_High;
                static ICACHE_RAM_ATTR void OnConfigPinChanged();
                static void connectWifi(const char* ssid, const char* password);

            };
        }
    }
}

#endif
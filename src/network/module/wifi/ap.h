#ifndef __NETWORK_MODULE_WIFI_AP_H__
#define __NETWORK_MODULE_WIFI_AP_H__

#include <string>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>
#include "rfir/util/button.h"
#include "rfir/util/event-emitter.h"

static const char Serial_Config_8N1[] PROGMEM = "8N1";
static const char Serial_Config_8N2[] PROGMEM = "8N2";
static const char Serial_Config_8O1[] PROGMEM = "8O1";
static const char Serial_Config_8O2[] PROGMEM = "8O2";
static const char Serial_Config_8E1[] PROGMEM = "8E1";
static const char Serial_Config_8E2[] PROGMEM = "8E2";

namespace network {
    namespace module {
        namespace wifi {
            class AP {
            public:
                struct Events {
                    rfir::util::Event configSaved;
                    rfir::util::Event applyDefault;
                };
                struct Params {
                    std::string apSsid;
                    std::string apPass;
                    std::string wifiSsid;
                    std::string wifiPass;
                    std::string configVersion = "0";
                    int         configPin = -1;
                    int         configPinTimeout = 5;
                    long        resetTimeout = 60 * 5;
                    int         statusPin = -1;
#ifdef ESP32                    
                    int         statusPinOnLevel = HIGH;
#else
                    int         statusPinOnLevel = LOW;
#endif                    
                };
            public:
                void initBtnStart();
                void initBtnStop();
                void* delayInitBtnStart(void* arg, void* p);
            protected:
                void handleRoot();                
            public:
                DNSServer* dnsServer;
                WebServer* webServer;
                IotWebConf* iotWebConf;                

                //Wifi
                static const uint8_t PROGMEM WifiSsid_max_length = 32;
                static const uint8_t PROGMEM WifiSsid_max_count = 20;
                char* wifiSsids = 0;
                char  wifiSsid[WifiSsid_max_length] = {};
                char  wifiPass[WifiSsid_max_length] = {};
                IotWebConfParameterGroup* wifiGroup = 0;
                IotWebConfSelectParameter* wifiParamSsid = 0;
                IotWebConfPasswordParameter* wifiParamPass = 0;

                //Serial
                static const uint8_t PROGMEM Serial_Band_max_length = 8;
                static const uint8_t PROGMEM Serial_Config_max_count = 6;

                char* serialConfigs = 0;
                char  serialBand[Serial_Band_max_length] = {};
                char  serialConfig[Serial_Band_max_length] = {};
                IotWebConfParameterGroup* serialGroup = 0;
                IotWebConfNumberParameter* serialParamBand = 0;
                IotWebConfSelectParameter* serialParamConfig = 0;
                
            public:
                Params params;
                Events events;
                rfir::util::Button::KeyTime keyTimeBtnStart = {pressed: 3000, released: 5000};
                rfir::util::Button::KeyTime keyTimeBtnStop = {pressed: 200, released: 1000};
            
            
            public:
                void start(Params p);
                void start();
                void stop();
                void loop();
            public:
                void init();
                void uninit();
                void initWebConf();
                void uninitWebConf();
                void initWifi();
                void uninitWifi();
                void initSerial();
                void uninitSerial();
                void applyDefault();
            public:
                void* onBtnStart(void* arg, void* p);
                void* onBtnStop(void* arg, void* p);
                void* onBtnLongPressed(void* arg, void* p);
                void* onBtnLongReleased(void* arg, void* p);
            public:
                static void connectWifi(const char* ssid, const char* password);
                static bool connectAp(const char* apName, const char* password);

            };
        }
    }
}

extern network::module::wifi::AP GWifiAP;

#endif
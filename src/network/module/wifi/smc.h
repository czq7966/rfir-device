#ifndef __NETWORK_MODULE_WIFI_SMC_H__
#define __NETWORK_MODULE_WIFI_SMC_H__

#include <string>
#include <Arduino.h>
#include <AceButton.h>
#include <jled.h>
#include "config.h"

#ifdef ESP8266  
#include "JustWifi.h" 
#define smc_messages_t justwifi_messages_t
#else
#define smc_messages_t uint16_t
#endif

namespace network {
    namespace module {
        namespace wifi {
            class SmcButton: public ace_button::AceButton {                
            public:
                SmcButton(uint8_t pin = 0, uint8_t defaultReleasedState = HIGH,  uint8_t id = 0);
            public:
                void* arg;
            };

            class SMC {
            public:
                struct Params {
                    std::string apSsid;
                    std::string apPass;
                    std::string wifiSsid;
                    std::string wifiPass;
                    std::string configVersion = "0";
                    std::string configFile = "/config/smc.json";
                    bool        smcIng = false;
                    int8_t        configPin = -1;
                    uint16_t      configPinType = 0;
                    int8_t        configPinNum = -1;
                    int8_t        configPinDelay = 0;
                    int8_t        configPinInterval = -1;
                    int8_t        configTimeout = 5;

                    void*          parent;                  
                };
            protected:
                long reset_timeout_start = 0;
                void handleRoot();                
            public:

                Params* params;
                SmcButton* button;
#ifdef  SMC_CONFIG_JLED               
                JLed led = SMC_CONFIG_JLED; 
#else                
                JLed led = JLed(BUILTIN_LED);
#endif                

            public:
                void start(Params* p);
                void loop();

                bool loadConfig();
                bool saveConfig();

                void setupConfigPin();
                void checkConfigPin();
                void checkSMC();
                void checkLED();
                void checkCallback();

                void setupSMC();
                void startSMC();
                void handleButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState);

                
            public:
                static unsigned long ConfigPinChangeTime;
                static          bool ConfigPinChanged;
                static unsigned long ConfigPinChangeTime_Low;
                static unsigned long ConfigPinChangeTime_High;
                static ICACHE_RAM_ATTR void OnConfigPinChanged();

                static smc_messages_t SmcLastMessage;
                static void SmcInfoWifi();
                static void SmcInfoCallback(smc_messages_t code, char * parameter);
                static void HandleAceButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState);

            };
        }
    }
}

extern network::module::wifi::SMC GSmartConfig;

#endif
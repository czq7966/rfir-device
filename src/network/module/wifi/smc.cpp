#include "smc.h"   
#include "client.h"
#include "config.h"     


network::module::wifi::SmcButton::SmcButton(uint8_t pin, uint8_t defaultReleasedState,  uint8_t id)
:ace_button::AceButton(pin, defaultReleasedState, id)
{
    
}

void network::module::wifi::SMC::start(Params* p) {
    Serial.println("SMC_start");
    this->params = p;
    this->setupSMC();
    this->setupConfigPin();
}

void network::module::wifi::SMC::loop() {
    checkConfigPin();
    checkSMC();
    checkLED();
}



void network::module::wifi::SMC::checkSMC() {
    if (this->params->smcIng) {
#ifdef ESP8266
        jw.loop();
#else
#endif        
        led.Update();
    }

}

void network::module::wifi::SMC::checkLED() {
    if (this->params->smcIng) {

    }
}

void network::module::wifi::SMC::applyDefault() {

}

unsigned long network::module::wifi::SMC::ConfigPinChangeTime = 0;
bool          network::module::wifi::SMC::ConfigPinChanged = false;
unsigned long network::module::wifi::SMC::ConfigPinChangeTime_Low = 0;
unsigned long network::module::wifi::SMC::ConfigPinChangeTime_High = 0;
void network::module::wifi::SMC::setupConfigPin() {
    if (this->params->configPin >=0) {
        Serial.print("configPin:");
        Serial.println(this->params->configPin);
        button = new SmcButton(this->params->configPin);
        button->arg = this;
        pinMode(this->params->configPin, INPUT_PULLUP);

        ace_button::ButtonConfig* buttonConfig = button->getButtonConfig();
        buttonConfig->setEventHandler(handleAceButtonEvent);
        if (this->params->configPinType > 0)
            buttonConfig->setFeature(this->params->configPinType);
        if (this->params->configPinDelay > 0) {
            switch (this->params->configPinType)
            {
                case ace_button::ButtonConfig::kFeatureClick:
                    buttonConfig->setClickDelay(this->params->configPinDelay);
                    break;
                case ace_button::ButtonConfig::kFeatureDoubleClick:
                    buttonConfig->setDoubleClickDelay(this->params->configPinDelay);
                    break;
                case ace_button::ButtonConfig::kFeatureLongPress:
                    buttonConfig->setLongPressDelay(this->params->configPinDelay);
                    break;
                case ace_button::ButtonConfig::kFeatureRepeatPress:
                    buttonConfig->setRepeatPressDelay(this->params->configPinDelay);
                    break;            
                default:
                    break;
            }
        }
    }
}

void network::module::wifi::SMC::handleButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState) {
    if (eventType == ace_button::AceButton::kEventPressed || eventType ==  ace_button::AceButton::kEventReleased)
        return;

    if (this->params->smcIng) {
#ifdef ESP8266                   
            ESP.reset();
#else
            ESP.restart();
#endif         
    } else {
        static uint8_t Button_Num = 0;
        static unsigned long Button_Event_Time = 0;

        if (millis() - Button_Event_Time > this->params->configPinInterval * 1000) {
            Button_Num = 0;
            Button_Event_Time = millis();
        }

        Button_Num++;

        if (Button_Num == this->params->configPinNum) {
            startSMC();

            Button_Num = 0;
            Button_Event_Time = millis();
        } 

    }


}

void network::module::wifi::SMC::handleAceButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState) {
    Serial.print(F("handleEvent(): eventType: "));
    Serial.print(eventType);
    Serial.print(F("; buttonState: "));
    Serial.println(buttonState);

    SmcButton* smcB = (SmcButton*)button;
    SMC* smc = (SMC*)(smcB->arg);
    smc->handleButtonEvent(button, eventType, buttonState);
}

void network::module::wifi::SMC::OnConfigPinChanged() {
    Serial.print("OnConfigPinChanged");
    ConfigPinChanged = true;
    ConfigPinChangeTime = millis();
}

void network::module::wifi::SMC::checkConfigPin() {
    if (this->button)
        this->button->check();
    // if (ConfigPinChanged) {
    //     ConfigPinChanged = false;
    //     bool pressed = digitalRead(this->params->configPin) == LOW;
    //     if (pressed) {
    //         ConfigPinChangeTime_Low = millis();
    //         Serial.println("Button pressed");            
    //     } else {
    //         ConfigPinChangeTime_High =  millis();
    //         Serial.println("Button released");            
    //         auto relay = ConfigPinChangeTime_High - ConfigPinChangeTime_Low;
    //         if ( ConfigPinChangeTime_Low > 0 && relay > this->params->configPinTimeStart * 1000 && relay < this->params->configPinTimeEnd * 1000) {
    //             //todo
    //             startSMC();
    //         }

    //         ConfigPinChangeTime = 0;            
    //     }

    // }
}

void network::module::wifi::SMC::setupSMC() {
#ifdef ESP8266    
    jw.enableAP(false);
    jw.enableAPFallback(false);
    jw.subscribe(smcInfoCallback);
#endif

}

void network::module::wifi::SMC::startSMC() {
    Serial.println("startSMC");
#ifdef ESP8266
    jw.startSmartConfig();
    this->params->smcIng = true;

#else        
    // startSMC_esp32();
#endif    
}


void network::module::wifi::SMC::smcInfoWifi() {


    if (WiFi.isConnected()) {

        uint8_t * bssid = WiFi.BSSID();

        Serial.printf("[WIFI] MODE STA -------------------------------------\n");
        Serial.printf("[WIFI] SSID  %s\n", WiFi.SSID().c_str());
        Serial.printf("[WIFI] PSW   %s\n", WiFi.psk().c_str());
        Serial.printf("[WIFI] BSSID %02X:%02X:%02X:%02X:%02X:%02X\n",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]
        );
        Serial.printf("[WIFI] CH    %d\n", WiFi.channel());
        Serial.printf("[WIFI] RSSI  %d\n", WiFi.RSSI());
        Serial.printf("[WIFI] IP    %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[WIFI] MAC   %s\n", WiFi.macAddress().c_str());
        Serial.printf("[WIFI] GW    %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("[WIFI] MASK  %s\n", WiFi.subnetMask().toString().c_str());
        Serial.printf("[WIFI] DNS   %s\n", WiFi.dnsIP().toString().c_str());
        #if defined(ARDUINO_ARCH_ESP32)
            Serial.printf("[WIFI] HOST  %s\n", WiFi.getHostname());
        #else
            Serial.printf("[WIFI] HOST  %s\n", WiFi.hostname().c_str());
        #endif
        Serial.printf("[WIFI] ----------------------------------------------\n");

    }

    if (WiFi.getMode() & WIFI_AP) {

        Serial.printf("[WIFI] MODE AP --------------------------------------\n");
        Serial.printf("[WIFI] SSID  %s\n", WiFi.softAPSSID().c_str());
        Serial.printf("[WIFI] IP    %s\n", WiFi.softAPIP().toString().c_str());
        Serial.printf("[WIFI] MAC   %s\n", WiFi.softAPmacAddress().c_str());
        Serial.printf("[WIFI] ----------------------------------------------\n");

    }

}


void network::module::wifi::SMC::smcInfoCallback(smc_messages_t code, char * parameter) {
#ifdef ESP8266
    // -------------------------------------------------------------------------

    if (code == MESSAGE_TURNING_OFF) {
        Serial.printf("[WIFI] Turning OFF\n");
    }

    if (code == MESSAGE_TURNING_ON) {
        Serial.printf("[WIFI] Turning ON\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_SCANNING) {
        Serial.printf("[WIFI] Scanning\n");
    }

    if (code == MESSAGE_SCAN_FAILED) {
        Serial.printf("[WIFI] Scan failed\n");
    }

    if (code == MESSAGE_NO_NETWORKS) {
        Serial.printf("[WIFI] No networks found\n");
    }

    if (code == MESSAGE_NO_KNOWN_NETWORKS) {
        Serial.printf("[WIFI] No known networks found\n");
    }

    if (code == MESSAGE_FOUND_NETWORK) {
        Serial.printf("[WIFI] %s\n", parameter);
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_CONNECTING) {
        Serial.printf("[WIFI] Connecting to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECT_WAITING) {
        // too much noise
    }

    if (code == MESSAGE_CONNECT_FAILED) {
        Serial.printf("[WIFI] Could not connect to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECTED) {
        smcInfoWifi();
    }

    if (code == MESSAGE_DISCONNECTED) {
        Serial.printf("[WIFI] Disconnected\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_ACCESSPOINT_CREATED) {
        smcInfoWifi();
    }

    if (code == MESSAGE_ACCESSPOINT_DESTROYED) {
        Serial.printf("[WIFI] Disconnecting access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_CREATING) {
        Serial.printf("[WIFI] Creating access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_FAILED) {
        Serial.printf("[WIFI] Could not create access point\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_WPS_START) {
        Serial.printf("[WIFI] WPS started\n");
    }

    if (code == MESSAGE_WPS_SUCCESS) {
        Serial.printf("[WIFI] WPS succeded!\n");
    }

    if (code == MESSAGE_WPS_ERROR) {
        Serial.printf("[WIFI] WPS failed\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_SMARTCONFIG_START) {
        Serial.printf("[WIFI] Smart Config started\n");
    }

    if (code == MESSAGE_SMARTCONFIG_SUCCESS) {
        Serial.printf("[WIFI] Smart Config succeded!\n");
    }

    if (code == MESSAGE_SMARTCONFIG_ERROR) {
        Serial.printf("[WIFI] Smart Config failed\n");
    }
#endif    
}


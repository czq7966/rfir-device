#include "smc.h"   
#include "client.h"
#include "config.h"     
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/util/file.h"


network::module::wifi::SmcButton::SmcButton(uint8_t pin, uint8_t defaultReleasedState,  uint8_t id)
:ace_button::AceButton(pin, defaultReleasedState, id)
{
    
}

void network::module::wifi::SMC::start(Params* p) {
    DEBUGER.println("SMC_start");
    this->params = p;
    this->setupSMC();
    this->setupConfigPin();
}

void network::module::wifi::SMC::loop() {
    checkConfigPin();
    if (this->params->smcIng) {
        checkSMC();
        checkLED();
        checkCallback();
    }
}

bool network::module::wifi::SMC::loadConfig() {
    neb::CJsonObject json;
    std::string fn = this->params->configFile;
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        json.Parse(context);
        std::string version;
        if (json.Get("configVersion", version) && version == this->params->configVersion) {
            json.Get("apSsid", this->params->apSsid);
            json.Get("apPass", this->params->apPass);
            json.Get("wifiSsid", this->params->wifiSsid);
            json.Get("wifiPass", this->params->wifiPass);     
            DEBUGER.println(this->params->wifiSsid.c_str());
            DEBUGER.println(this->params->wifiPass.c_str());
            return true;       
        }
    } 

    return false;
}

bool network::module::wifi::SMC::saveConfig() {
    neb::CJsonObject json;
    std::string fn = this->params->configFile;
    rfir::util::TxtFile file(fn.c_str());
    std::string context;

    json.Add("apSsid", this->params->apSsid);
    json.Add("apPass", this->params->apPass);
    json.Add("wifiSsid", this->params->wifiSsid);
    json.Add("wifiPass", this->params->wifiPass);
    json.Add("configVersion", this->params->configVersion);

    context = json.ToString();
    return file.writeString(context);
}

void network::module::wifi::SMC::checkSMC() {
#ifdef ESP8266
    jw.loop();
#endif        
}

void network::module::wifi::SMC::checkLED() {
    led.Update();
}

void network::module::wifi::SMC::checkCallback() {
#ifdef ESP8266

    if (SmcLastMessage == MESSAGE_SMARTCONFIG_ERROR) {
        this->params->configTimeout--;
        if (this->params->configTimeout <= 0) {
            ESP.reset();
        } else {
            jw.startSmartConfig();
        }
    }

    if (SmcLastMessage == MESSAGE_SMARTCONFIG_START) {

    }

    if (SmcLastMessage == MESSAGE_SMARTCONFIG_SUCCESS) {
        this->params->wifiSsid = WiFi.SSID().c_str();
        this->params->wifiPass = WiFi.psk().c_str();
        saveConfig();
        delay(1000);
        ESP.reset();
    }    


#endif        
}

unsigned long network::module::wifi::SMC::ConfigPinChangeTime = 0;
bool          network::module::wifi::SMC::ConfigPinChanged = false;
unsigned long network::module::wifi::SMC::ConfigPinChangeTime_Low = 0;
unsigned long network::module::wifi::SMC::ConfigPinChangeTime_High = 0;
#ifdef ESP8266
smc_messages_t network::module::wifi::SMC::SmcLastMessage = MESSAGE_SCANNING;
#else
smc_messages_t network::module::wifi::SMC::SmcLastMessage = 0;
#endif
void network::module::wifi::SMC::setupConfigPin() {
    if (this->params->configPin >=0) {
        DEBUGER.print("configPin:");
        DEBUGER.println(this->params->configPin);
        button = new SmcButton(this->params->configPin);
        button->arg = this;
        pinMode(this->params->configPin, INPUT_PULLUP);

        ace_button::ButtonConfig* buttonConfig = button->getButtonConfig();
        buttonConfig->setEventHandler(HandleAceButtonEvent);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureLongPress);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureRepeatPress);
  return;
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
    DEBUGER.println("network::module::wifi::SMC::handleButtonEvent");
    if (eventType == ace_button::AceButton::kEventPressed || eventType ==  ace_button::AceButton::kEventReleased) {
        DEBUGER.println("kEventPressed or kEventReleased");
        return;
    }

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

void network::module::wifi::SMC::HandleAceButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState) {
    DEBUGER.print(F("handleEvent(): eventType: "));
    DEBUGER.print(eventType);
    DEBUGER.print(F("; buttonState: "));
    DEBUGER.println(buttonState);

    SmcButton* smcB = (SmcButton*)button;
    SMC* smc = (SMC*)(smcB->arg);
    smc->handleButtonEvent(button, eventType, buttonState);
}

void network::module::wifi::SMC::OnConfigPinChanged() {
    DEBUGER.print("OnConfigPinChanged");
    ConfigPinChanged = true;
    ConfigPinChangeTime = millis();
}

void network::module::wifi::SMC::checkConfigPin() {
    if (this->button) {
        this->button->check();
    }
}

void network::module::wifi::SMC::setupSMC() {
#ifdef ESP8266    
    jw.enableAP(false);
    jw.enableAPFallback(false);
    jw.subscribe(SmcInfoCallback);
#endif
    loadConfig();
    Client::Params* cp = (Client::Params*)this->params->parent;
    if (this->params->wifiSsid.length() > 0) {
        cp->ssid.insert(cp->ssid.begin(), this->params->wifiSsid);
        cp->pass.insert(cp->pass.begin(), this->params->wifiPass);
    }

    if (cp->ssid.size() == 0) {
        startSMC();
    }
}

void network::module::wifi::SMC::startSMC() {
    DEBUGER.println("startSMC");
#ifdef ESP8266
    this->params->smcIng = true;
    jw.startSmartConfig();
#endif    
}


void network::module::wifi::SMC::SmcInfoWifi() {


    if (WiFi.isConnected()) {

        uint8_t * bssid = WiFi.BSSID();

        DEBUGER.printf("[WIFI] MODE STA -------------------------------------\n");
        DEBUGER.printf("[WIFI] SSID  %s\n", WiFi.SSID().c_str());
        DEBUGER.printf("[WIFI] PSW   %s\n", WiFi.psk().c_str());
        DEBUGER.printf("[WIFI] BSSID %02X:%02X:%02X:%02X:%02X:%02X\n",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]
        );
        DEBUGER.printf("[WIFI] CH    %d\n", WiFi.channel());
        DEBUGER.printf("[WIFI] RSSI  %d\n", WiFi.RSSI());
        DEBUGER.printf("[WIFI] IP    %s\n", WiFi.localIP().toString().c_str());
        DEBUGER.printf("[WIFI] MAC   %s\n", WiFi.macAddress().c_str());
        DEBUGER.printf("[WIFI] GW    %s\n", WiFi.gatewayIP().toString().c_str());
        DEBUGER.printf("[WIFI] MASK  %s\n", WiFi.subnetMask().toString().c_str());
        DEBUGER.printf("[WIFI] DNS   %s\n", WiFi.dnsIP().toString().c_str());
        #if defined(ARDUINO_ARCH_ESP32)
            DEBUGER.printf("[WIFI] HOST  %s\n", WiFi.getHostname());
        #else
            DEBUGER.printf("[WIFI] HOST  %s\n", WiFi.hostname().c_str());
        #endif
        DEBUGER.printf("[WIFI] ----------------------------------------------\n");

    }

    if (WiFi.getMode() & WIFI_AP) {

        DEBUGER.printf("[WIFI] MODE AP --------------------------------------\n");
        DEBUGER.printf("[WIFI] SSID  %s\n", WiFi.softAPSSID().c_str());
        DEBUGER.printf("[WIFI] IP    %s\n", WiFi.softAPIP().toString().c_str());
        DEBUGER.printf("[WIFI] MAC   %s\n", WiFi.softAPmacAddress().c_str());
        DEBUGER.printf("[WIFI] ----------------------------------------------\n");

    }

}


void network::module::wifi::SMC::SmcInfoCallback(smc_messages_t code, char * parameter) {
#ifdef ESP8266
    SmcLastMessage = code;
    // -------------------------------------------------------------------------

    if (code == MESSAGE_TURNING_OFF) {
        DEBUGER.printf("[WIFI] Turning OFF\n");
    }

    if (code == MESSAGE_TURNING_ON) {
        DEBUGER.printf("[WIFI] Turning ON\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_SCANNING) {
        DEBUGER.printf("[WIFI] Scanning\n");
    }

    if (code == MESSAGE_SCAN_FAILED) {
        DEBUGER.printf("[WIFI] Scan failed\n");
    }

    if (code == MESSAGE_NO_NETWORKS) {
        DEBUGER.printf("[WIFI] No networks found\n");
    }

    if (code == MESSAGE_NO_KNOWN_NETWORKS) {
        DEBUGER.printf("[WIFI] No known networks found\n");
    }

    if (code == MESSAGE_FOUND_NETWORK) {
        DEBUGER.printf("[WIFI] %s\n", parameter);
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_CONNECTING) {
        DEBUGER.printf("[WIFI] Connecting to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECT_WAITING) {
        // too much noise
    }

    if (code == MESSAGE_CONNECT_FAILED) {
        DEBUGER.printf("[WIFI] Could not connect to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECTED) {
        SmcInfoWifi();
    }

    if (code == MESSAGE_DISCONNECTED) {
        DEBUGER.printf("[WIFI] Disconnected\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_ACCESSPOINT_CREATED) {
        SmcInfoWifi();
    }

    if (code == MESSAGE_ACCESSPOINT_DESTROYED) {
        DEBUGER.printf("[WIFI] Disconnecting access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_CREATING) {
        DEBUGER.printf("[WIFI] Creating access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_FAILED) {
        DEBUGER.printf("[WIFI] Could not create access point\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_WPS_START) {
        DEBUGER.printf("[WIFI] WPS started\n");
    }

    if (code == MESSAGE_WPS_SUCCESS) {
        DEBUGER.printf("[WIFI] WPS succeded!\n");
    }

    if (code == MESSAGE_WPS_ERROR) {
        DEBUGER.printf("[WIFI] WPS failed\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_SMARTCONFIG_START) {
        DEBUGER.printf("[WIFI] Smart Config started\n");
    }

    if (code == MESSAGE_SMARTCONFIG_SUCCESS) {
        DEBUGER.printf("[WIFI] Smart Config succeded!\n");
    }

    if (code == MESSAGE_SMARTCONFIG_ERROR) {
        DEBUGER.printf("[WIFI] Smart Config failed\n");
    }
#endif    
}


network::module::wifi::SMC GSmartConfig;

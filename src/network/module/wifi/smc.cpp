#include "smc.h"   
#include "client.h"
#include "config.h"     
#include "rfir/util/file.h"
#include "rfir/util/util.h"


network::module::wifi::SmcButton::SmcButton(uint8_t pin, uint8_t defaultReleasedState,  uint8_t id)
:ace_button::AceButton(pin, defaultReleasedState, id)
{
    
}

void network::module::wifi::SMC::start(Params* p) {
    GDebuger.println(F("SMC_start"));
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
    std::string fn = this->params->configFile;
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        DynamicJsonDocument doc(Config.props.mqtt_buffer_size);
        deserializeJson(doc, context);
        JsonObject json = doc.as<JsonObject>();
        
        if(json.containsKey("configVersion")) {
            std::string version = json["configVersion"];
            if (version == this->params->configVersion) {
                this->params->apSsid = json.containsKey("apSsid") ? json["apSsid"].as<std::string>() : this->params->apSsid;
                this->params->apPass = json.containsKey("apPass") ? json["apPass"].as<std::string>() : this->params->apPass;
                this->params->wifiSsid = json.containsKey("wifiSsid") ? json["wifiSsid"].as<std::string>() : this->params->wifiSsid;
                this->params->wifiPass = json.containsKey("wifiPass") ? json["wifiPass"].as<std::string>() : this->params->wifiPass;
                return true;           
            }
        }
    } 

    return false;
}

bool network::module::wifi::SMC::saveConfig() {
    DynamicJsonDocument doc(Config.props.mqtt_buffer_size);
    doc["apSsid"] = this->params->apSsid;
    doc["apPass"] = this->params->apPass;
    doc["wifiSsid"] = this->params->wifiSsid;
    doc["wifiPass"] = this->params->wifiPass;
    doc["configVersion"] = this->params->configVersion;
    std::string context;
    serializeJson(doc, context);

    std::string fn = this->params->configFile;
    rfir::util::TxtFile file(fn.c_str());
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
            rfir::util::Util::Reset();
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
        rfir::util::Util::Reset();
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
        GDebuger.print("configPin:");
        GDebuger.println(this->params->configPin);
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
    GDebuger.println(F("network::module::wifi::SMC::handleButtonEvent"));
    if (eventType == ace_button::AceButton::kEventPressed || eventType ==  ace_button::AceButton::kEventReleased) {
        GDebuger.println(F("kEventPressed or kEventReleased"));
        return;
    }

    if (this->params->smcIng) {
        rfir::util::Util::Reset();        
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
    GDebuger.print(F("handleEvent(): eventType: "));
    GDebuger.print(eventType);
    GDebuger.print(F("; buttonState: "));
    GDebuger.println(buttonState);

    SmcButton* smcB = (SmcButton*)button;
    SMC* smc = (SMC*)(smcB->arg);
    smc->handleButtonEvent(button, eventType, buttonState);
}

void network::module::wifi::SMC::OnConfigPinChanged() {
    GDebuger.print("OnConfigPinChanged");
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
    GDebuger.println(F("startSMC"));
#ifdef ESP8266
    this->params->smcIng = true;
    jw.startSmartConfig();
#endif    
}


void network::module::wifi::SMC::SmcInfoWifi() {


    if (WiFi.isConnected()) {

        uint8_t * bssid = WiFi.BSSID();

        GDebuger.printf("[WIFI] MODE STA -------------------------------------\n");
        GDebuger.printf("[WIFI] SSID  %s\n", WiFi.SSID().c_str());
        GDebuger.printf("[WIFI] PSW   %s\n", WiFi.psk().c_str());
        GDebuger.printf("[WIFI] BSSID %02X:%02X:%02X:%02X:%02X:%02X\n",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]
        );
        GDebuger.printf("[WIFI] CH    %d\n", WiFi.channel());
        GDebuger.printf("[WIFI] RSSI  %d\n", WiFi.RSSI());
        GDebuger.printf("[WIFI] IP    %s\n", WiFi.localIP().toString().c_str());
        GDebuger.printf("[WIFI] MAC   %s\n", WiFi.macAddress().c_str());
        GDebuger.printf("[WIFI] GW    %s\n", WiFi.gatewayIP().toString().c_str());
        GDebuger.printf("[WIFI] MASK  %s\n", WiFi.subnetMask().toString().c_str());
        GDebuger.printf("[WIFI] DNS   %s\n", WiFi.dnsIP().toString().c_str());
        #if defined(ARDUINO_ARCH_ESP32)
            GDebuger.printf("[WIFI] HOST  %s\n", WiFi.getHostname());
        #else
            GDebuger.printf("[WIFI] HOST  %s\n", WiFi.hostname().c_str());
        #endif
        GDebuger.printf("[WIFI] ----------------------------------------------\n");

    }

    if (WiFi.getMode() & WIFI_AP) {

        GDebuger.printf("[WIFI] MODE AP --------------------------------------\n");
        GDebuger.printf("[WIFI] SSID  %s\n", WiFi.softAPSSID().c_str());
        GDebuger.printf("[WIFI] IP    %s\n", WiFi.softAPIP().toString().c_str());
        GDebuger.printf("[WIFI] MAC   %s\n", WiFi.softAPmacAddress().c_str());
        GDebuger.printf("[WIFI] ----------------------------------------------\n");

    }

}


void network::module::wifi::SMC::SmcInfoCallback(smc_messages_t code, char * parameter) {
#ifdef ESP8266
    SmcLastMessage = code;
    // -------------------------------------------------------------------------

    if (code == MESSAGE_TURNING_OFF) {
        GDebuger.printf("[WIFI] Turning OFF\n");
    }

    if (code == MESSAGE_TURNING_ON) {
        GDebuger.printf("[WIFI] Turning ON\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_SCANNING) {
        GDebuger.printf("[WIFI] Scanning\n");
    }

    if (code == MESSAGE_SCAN_FAILED) {
        GDebuger.printf("[WIFI] Scan failed\n");
    }

    if (code == MESSAGE_NO_NETWORKS) {
        GDebuger.printf("[WIFI] No networks found\n");
    }

    if (code == MESSAGE_NO_KNOWN_NETWORKS) {
        GDebuger.printf("[WIFI] No known networks found\n");
    }

    if (code == MESSAGE_FOUND_NETWORK) {
        GDebuger.printf("[WIFI] %s\n", parameter);
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_CONNECTING) {
        GDebuger.printf("[WIFI] Connecting to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECT_WAITING) {
        // too much noise
    }

    if (code == MESSAGE_CONNECT_FAILED) {
        GDebuger.printf("[WIFI] Could not connect to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECTED) {
        SmcInfoWifi();
    }

    if (code == MESSAGE_DISCONNECTED) {
        GDebuger.printf("[WIFI] Disconnected\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_ACCESSPOINT_CREATED) {
        SmcInfoWifi();
    }

    if (code == MESSAGE_ACCESSPOINT_DESTROYED) {
        GDebuger.printf("[WIFI] Disconnecting access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_CREATING) {
        GDebuger.printf("[WIFI] Creating access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_FAILED) {
        GDebuger.printf("[WIFI] Could not create access point\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_WPS_START) {
        GDebuger.printf("[WIFI] WPS started\n");
    }

    if (code == MESSAGE_WPS_SUCCESS) {
        GDebuger.printf("[WIFI] WPS succeded!\n");
    }

    if (code == MESSAGE_WPS_ERROR) {
        GDebuger.printf("[WIFI] WPS failed\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_SMARTCONFIG_START) {
        GDebuger.printf("[WIFI] Smart Config started\n");
    }

    if (code == MESSAGE_SMARTCONFIG_SUCCESS) {
        GDebuger.printf("[WIFI] Smart Config succeded!\n");
    }

    if (code == MESSAGE_SMARTCONFIG_ERROR) {
        GDebuger.printf("[WIFI] Smart Config failed\n");
    }
#endif    
}


network::module::wifi::SMC GSmartConfig;

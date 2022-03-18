#include "ap.h"   
#include "config.h"        

void network::module::wifi::AP::start(Params p) {
    this->params = p;

    this->dnsServer = new DNSServer();
    this->webServer = new WebServer(80);
    this->iotWebConf = new IotWebConf(this->params.apSsid.c_str(), this->dnsServer, this->webServer, this->params.apPass.c_str(), this->params.configVersion.c_str());

    DEBUGER.printf("AP Starting up...%s %s\n:", this->params.apSsid.c_str(), this->params.apPass.c_str());
    this->iotWebConf->setStatusPin(this->params.statusPin, this->params.statusPinOnLevel);
    this->iotWebConf->setConfigPin(this->params.configPin);
    this->iotWebConf->setWifiConnectionHandler(&connectWifi);

    auto valid = this->iotWebConf->init();
    if (!valid) this->applyDefault();

    this->webServer->on("/", [this]{ this->handleRoot(); });
    this->webServer->on("/config", [this]{ this->iotWebConf->handleConfig(); });
    this->webServer->onNotFound([this]{ this->iotWebConf->handleNotFound(); });

    this->setupConfigPin();
    DEBUGER.println("AP Ready.");
}

void network::module::wifi::AP::loop() {
    checkOrReset();
    checkConfigPin();
    this->iotWebConf->doLoop();
}

void network::module::wifi::AP::handleRoot() {
    if (this->iotWebConf->handleCaptivePortal())
    {
        // -- Captive portal request were already served.
        return;
    }
    String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    s += "<title>IotWebConf 01 Minimal</title></head><body>";
    s += "Go to <a href='config'>configure page</a> to change settings.";
    s += "</body></html>\n";

    this->webServer->send(200, "text/html", s);
}


void network::module::wifi::AP::checkOrReset() {
    if (WiFi.status() != WL_CONNECTED) {
        if (reset_timeout_start == 0 )
            reset_timeout_start = millis();
        if (millis() - reset_timeout_start > this->params.resetTimeout * 1000)
        {
            DEBUGER.println("WiFi connect time out. ESP rest!\n");
            delay(1000);
#ifdef ESP8266                   
            ESP.reset();
#else
            ESP.restart();
#endif                
            return;
        }
        
        yield();

    } else {
        if (reset_timeout_start != 0) {
            reset_timeout_start = 0;

            DEBUGER.println("");
            DEBUGER.println("WiFi connected\n");
            
            DEBUGER.println("========= WIFI CONNECT OK! Led Light! ==============\n");
            IPAddress ip = WiFi.localIP();
            String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
            DEBUGER.println("IP Address:" + ipStr);
        }
    }

}

void network::module::wifi::AP::applyDefault() {
    this->iotWebConf->getThingNameParameter()->defaultValue = this->params.apSsid.c_str();
    this->iotWebConf->getThingNameParameter()->applyDefaultValue();

    this->iotWebConf->getApPasswordParameter()->defaultValue = this->params.apPass.c_str();
    this->iotWebConf->getApPasswordParameter()->applyDefaultValue();

    this->iotWebConf->getWifiSsidParameter()->defaultValue = this->params.wifiSsid.c_str();
    this->iotWebConf->getWifiSsidParameter()->applyDefaultValue();

    this->iotWebConf->getWifiPasswordParameter()->defaultValue = this->params.wifiPass.c_str();
    this->iotWebConf->getWifiPasswordParameter()->applyDefaultValue();
    DEBUGER.println("Use defaults: ");
     DEBUGER.printf("---AP   Ssid: %s\n", this->params.apSsid.c_str());
     DEBUGER.printf("---AP   Pass: %s\n", this->params.apPass.c_str());
     DEBUGER.printf("---WiFi Ssid: %s\n", this->params.wifiSsid.c_str());
     DEBUGER.printf("---WiFi Pass: %s\n", this->params.wifiPass.c_str());
}

long network::module::wifi::AP::ConfigPinChangeTime = 0;
long network::module::wifi::AP::ConfigPinChangeTime_Low = 0;
long network::module::wifi::AP::ConfigPinChangeTime_High = 0;
void network::module::wifi::AP::setupConfigPin() {
    if (this->params.configPin >=0) {
        pinMode(this->params.configPin, INPUT_PULLUP);
        attachInterrupt(this->params.configPin, OnConfigPinChanged , CHANGE);        
    }
}

void network::module::wifi::AP::OnConfigPinChanged() {
    ConfigPinChangeTime = millis();
}

void network::module::wifi::AP::checkConfigPin() {
    if (ConfigPinChangeTime > 0) {
        if (digitalRead(this->params.configPin) == LOW) {
            ConfigPinChangeTime_Low = ConfigPinChangeTime_Low == 0 ? millis() : ConfigPinChangeTime_Low;

            if ( millis() - ConfigPinChangeTime >= this->params.configPinTimeout * 1000) {
                ConfigPinChangeTime = 0;
                DEBUGER.println("ConfigPin pushed, timeout, ESP will reset");
                delay(500);
                #ifdef ESP8266            
                    ESP.reset();
                #else
                    ESP.restart();
                #endif           
            }
        } else {
            ConfigPinChangeTime_High =  millis();            
            auto relay = ConfigPinChangeTime_High - ConfigPinChangeTime_Low;
            if ( ConfigPinChangeTime_Low > 0 && relay > 500 && relay < this->params.configPinTimeout * 1000) {
                if (this->iotWebConf->getState() == iotwebconf::NetworkState::NotConfigured) {
                    DEBUGER.println("ConfigPin pushed, NotConfigured, ESP will reset");
                    delay(500);                
                    #ifdef ESP8266            
                        ESP.reset();
                    #else
                        ESP.restart();
                    #endif 
                }
            }

            ConfigPinChangeTime = 0;            
        }

    }
}



void network::module::wifi::AP::connectWifi(const char* ssid, const char* password)
{
    //临时解决办法：用延时来临时解决WiFi启动卡死的问题
    DEBUGER.println("WiFi.setAutoConnect");
    delay(1000);   
    WiFi.setAutoConnect(true);
#ifdef ESP8266     
    DEBUGER.println("WiFi.setPhyMode(WIFI_PHY_MODE_11B)");
    delay(1000);
    WiFi.setPhyMode(WIFI_PHY_MODE_11B);
#endif  
    DEBUGER.println("WiFi.begin......");
    delay(1000);
    WiFi.begin(ssid, password);
    DEBUGER.println("WiFi.begined");
}
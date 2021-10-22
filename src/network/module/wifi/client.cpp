#include "client.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

void network::module::wifi::Client::start(Params p) {
    this->params = p;
    this->checkOrReset();

    // auto* mng = (ESPAsync_WiFiManager_Lite*) this->manager;
    // if (!mng) {
    //     mng = new ESPAsync_WiFiManager_Lite();
    //     this->manager = (void*) mng;
    // }

    // mng->setConfigPortalChannel(0);
    // mng->begin();
}

void network::module::wifi::Client::loop() {
    // auto mng = (ESPAsync_WiFiManager_Lite*) this->manager;
    // if (mng) 
    //     mng->run();        
    checkOrReset();
}

void network::module::wifi::Client::checkOrReset(long timeout) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connecting......");
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        WiFi.setAutoConnect(true);
#ifdef ESP8266     
        WiFi.setPhyMode(WIFI_PHY_MODE_11B);
#endif        
        delay(1000);
        WiFi.begin(params.ssid.c_str(), params.pass.c_str());
        long start = millis();
        while (WiFi.status() != WL_CONNECTED)
        {
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
            if (millis() - start > timeout * 1000)
            {
                Serial.println("WiFi connect time out. ESP rest!\n");
                delay(1000);
#ifdef ESP8266                   
                ESP.reset();
#else
                ESP.restart();
#endif                
                return;
            }
            
            yield();
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            Serial.print(".");
        }

        Serial.println("");
        Serial.println("WiFi connected\n");
        
        Serial.println("========= WIFI CONNECT OK! Led Light! ==============\n");
        IPAddress ip = WiFi.localIP();
        String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
        Serial.println("IP Address:" + ipStr);
        digitalWrite(LED_BUILTIN, HIGH);

    }


}

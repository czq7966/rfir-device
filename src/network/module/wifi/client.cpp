#include "client.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

bool network::module::wifi::Client::Params::assign(Params& p) {
    this->apMode = p.apMode;
    this->ap = p.ap;
    this->ssid.assign(p.ssid.begin(), p.ssid.end());
    this->pass.assign(p.pass.begin(), p.pass.end());
    this->timeout.assign(p.timeout.begin(), p.timeout.end());
    return true;
}

void network::module::wifi::Client::start(Params& p) {
    this->params.assign(p);
    // this->checkOrReset();
    this->multiCheckOrReset();
}

void network::module::wifi::Client::loop() {
    // checkOrReset();
    this->multiCheckOrReset();
}

void network::module::wifi::Client::checkOrReset() {
//     if (!this->params.apMode && WiFi.status() != WL_CONNECTED ) {
//         Serial.println("WiFi connecting......");
//         pinMode(LED_BUILTIN, OUTPUT);
//         digitalWrite(LED_BUILTIN, HIGH);
//         WiFi.setAutoConnect(true);
// #ifdef ESP8266     
//         WiFi.setPhyMode(WIFI_PHY_MODE_11B);
// #endif        
//         delay(1000);
//         WiFi.begin(params.ssid.c_str(), params.pass.c_str());
//         long start = millis();
//         while (WiFi.status() != WL_CONNECTED)
//         {
//             digitalWrite(LED_BUILTIN, LOW);
//             delay(500);
//             if (millis() - start > this->params.resetTimeout * 1000)
//             {
//                 Serial.println("WiFi connect time out. ESP rest!\n");
//                 delay(1000);
// #ifdef ESP8266                   
//                 ESP.reset();
// #else
//                 ESP.restart();
// #endif                
//                 return;
//             }
            
//             yield();
//             digitalWrite(LED_BUILTIN, HIGH);
//             delay(500);
//             Serial.print(".");
//         }

//         Serial.println("");
//         Serial.println("WiFi connected\n");
        
//         Serial.println("========= WIFI CONNECT OK! Led Light! ==============\n");
//         IPAddress ip = WiFi.localIP();
//         String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
//         Serial.println("IP Address:" + ipStr);
//         digitalWrite(LED_BUILTIN, HIGH);

//     }


}

void network::module::wifi::Client::multiCheckOrReset() {
    if (!this->params.apMode && WiFi.status() != WL_CONNECTED ) {
        Serial.println("WiFi connecting......");
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        WiFi.setAutoConnect(true);
#ifdef ESP8266     
        WiFi.setPhyMode(WIFI_PHY_MODE_11B);
#endif        
        delay(1000);
        
        for (size_t i = 0; i < this->params.ssid.size(); i++)
        {
            std::string ssid = this->params.ssid[i];
            std::string pass = this->params.pass[i];
            int         timeout = this->params.timeout[i];
            if (connectWifi(ssid, pass, timeout )) {
                break;
            }
        }

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi connect time out. ESP rest!\n");
            delay(1000);
#ifdef ESP8266                   
            ESP.reset();
#else
            ESP.restart();
#endif                
            return;           
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

bool network::module::wifi::Client::connectWifi(std::string ssid, std::string pass, int timeout) {
    Serial.printf("WiFi connecting......: %s %s %d \r\n", ssid.c_str(), pass.c_str(), timeout);
    WiFi.disconnect();
    WiFi.begin(ssid.c_str(), pass.c_str());
    long start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        if (millis() - start > timeout * 1000)
        {               
            break;
        }
        
        yield();
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        Serial.print(".");
    }
    return WiFi.status() == WL_CONNECTED;
}
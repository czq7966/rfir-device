#include "client.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

void network::module::wifi::Client::start(Params p) {
    WiFi.begin(p.ssid.c_str(), p.pass.c_str());
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
}
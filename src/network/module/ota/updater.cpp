#include "updater.h"
#include "rfir/util/event-timer.h"

void network::module::ota::Updater::start(Params p) {
    params = p;
    if (!eOtaUpdater) { 
        eOtaUpdater = new EOTAUpdate(String(p.url.c_str()),  String(p.id.c_str()), p.version);    
    }

    wifiConnectHandler = WiFi.onStationModeGotIP(std::bind(&Updater::onWifiConnect, this, std::placeholders::_1));
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(std::bind(&Updater::onWifiDisconnect, this, std::placeholders::_1));    
}

void network::module::ota::Updater::loop() {
 
}

void network::module::ota::Updater::onWifiConnect(const WiFiEventStationModeGotIP& event) {
    if (m_update_handler == 0) {
        m_update_handler = GEventTimer.delay(1000, std::bind(&Updater::doCheckAndUpdate, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void network::module::ota::Updater::onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {    

}

void network::module::ota::Updater::checkAndUpdate(){
    if (eOtaUpdater) 
       eOtaUpdater->CheckAndUpdate(true);     
};

void* network::module::ota::Updater::doCheckAndUpdate(void* arg, void* p){
    checkAndUpdate();
    m_update_handler = GEventTimer.delay(params.interval * 1000, std::bind(&Updater::doCheckAndUpdate, this, std::placeholders::_1, std::placeholders::_2));
    return 0;
};

network::module::ota::Updater GOTAUpdater;
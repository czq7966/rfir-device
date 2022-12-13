#include "updater.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "../wifi/client.h"

void network::module::ota::Updater::start() {
    if (!this->params.enabled)
        return;

    if (!eOtaUpdater) { 
        eOtaUpdater = new EOTAUpdate(String(this->params.url),  String(this->params.id), this->params.version);    
    }

    GWifiClient.events.onWifiConnect.add(this, std::bind(&Updater::onWifiConnect, this, std::placeholders::_1, std::placeholders::_2));
    GWifiClient.events.onWifiDisconnect.add(this, std::bind(&Updater::onWifiDisconnect, this, std::placeholders::_1, std::placeholders::_2));    
}

void network::module::ota::Updater::loop() {
 
}

void* network::module::ota::Updater::onWifiConnect(void* arg, void* p) {
    if (m_update_handler == 0) {
        m_update_handler = GEventTimer.delay(params.interval, std::bind(&Updater::doCheckAndUpdate, this, std::placeholders::_1, std::placeholders::_2));
    }
    return 0;
}

void* network::module::ota::Updater::onWifiDisconnect(void* arg, void* p) {    
    return 0;
}

void network::module::ota::Updater::checkAndUpdate(){
    if (eOtaUpdater) 
       eOtaUpdater->CheckAndUpdate(true);     
};

void* network::module::ota::Updater::doCheckAndUpdate(void* arg, void* p){
    checkAndUpdate();
    if (this->params.reboot)
        rfir::util::Util::Reset();
    m_update_handler = GEventTimer.delay(params.interval, std::bind(&Updater::doCheckAndUpdate, this, std::placeholders::_1, std::placeholders::_2));
    return 0;
};

network::module::ota::Updater GOTAUpdater;
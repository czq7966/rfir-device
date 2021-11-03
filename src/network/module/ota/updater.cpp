#include "updater.h"

void network::module::ota::Updater::start(Params p) {
    if (!eOtaUpdater) 
        eOtaUpdater = new EOTAUpdate(String(p.url.c_str()),  String(p.id.c_str()), p.version);    
}

void network::module::ota::Updater::loop() {
    if (eOtaUpdater) 
        eOtaUpdater->CheckAndUpdate();     
}

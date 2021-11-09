#include "updater.h"

network::module::ota::Updater* network::service::ota::Updater::updater = 0;

void network::service::ota::Updater::Start(network::module::ota::Updater::Params p) {
    if (!updater)
        updater = new network::module::ota::Updater();
    updater->start(p);
}


void network::service::ota::Updater::Loop() {
    if (updater && (WiFi.status() == WL_CONNECTED))
        updater->loop();
}

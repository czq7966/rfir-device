
#ifndef __NETWORK_SERVICE_OTA_UPDATER_H__
#define __NETWORK_SERVICE_OTA_UPDATER_H__

#ifdef ESP8266

#include "rfir/util/platform.h"
#include "network/module/ota/updater.h"

namespace network {
    namespace service {
        namespace ota {
            class Updater {
            public:
                static network::module::ota::Updater* updater;
            public:
                static void Start(network::module::ota::Updater::Params p);
                static void Loop();
            };

        }
    }
}

#endif


#endif //__NETWORK_SERVICE_OTA_UPDATER_H__

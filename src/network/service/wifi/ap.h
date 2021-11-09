#ifndef __NETWORK_SERVICE_WIFI_AP_H__
#define __NETWORK_SERVICE_WIFI_AP_H__


#include "network/module/wifi/ap.h"

namespace network {
    namespace service {
        namespace wifi {
            class AP {
            private:
                static network::module::wifi::AP* ap;

            public:
                static void Start(module::wifi::AP::Params p);
                static void Loop();

            };
        }
    }
}

#endif
#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include "ap.h"

namespace network {
    namespace module {
        namespace wifi {
            class Client {
            public:
                struct Params {
                    bool        apMode = false;
                    std::string ssid;
                    std::string pass;
                    long        resetTimeout = 60;
                    AP::Params  ap;
                };
            public:
                Params  params;
            public:
                void start(Params p);
                void loop();
                void checkOrReset();

            };
        }
    }
}

#endif
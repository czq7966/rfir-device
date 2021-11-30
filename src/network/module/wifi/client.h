#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include <vector>
#include "ap.h"

namespace network {
    namespace module {
        namespace wifi {
            class Client {
            public:
                struct Params {
                    bool                apMode = false;
                    std::vector<std::string>    ssid;
                    std::vector<std::string>    pass;                   
                    std::vector<int>            timeout;
                    AP::Params          ap;
                    bool                assign(Params& p);
                };
            public:
                Params  params;
            public:
                void start(Params& p);
                void loop();
                void checkOrReset();
                void multiCheckOrReset();
                bool connectWifi(std::string ssid, std::string pass, int timeout);

            };
        }
    }
}

#endif
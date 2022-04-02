#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

#include <string>
#include <vector>
#include "ap.h"
#include "smc.h"

namespace network {
    namespace module {
        namespace wifi {
            class Client {
            public:
                struct Params {
                    bool                apMode = false;
                    bool                smcMode= false;
                    std::vector<std::string>    ssid;
                    std::vector<std::string>    pass;                   
                    std::vector<int>            timeout;
                    AP::Params          ap;
                    SMC::Params         smc;
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
                int  connectWifi();
                void multiCheckOrReset2();

            };
        }
    }
}

#endif
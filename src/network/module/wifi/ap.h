#ifndef __RFIR_MODULE_WIFI_AP_H__
#define __RFIR_MODULE_WIFI_AP_H__

#include <string>

namespace network {
    namespace module {
        namespace wifi {
            class AP {
            public:
                struct Params {
                    std::string ssid;
                    std::string pass;
                };
            private:
                void* manager;
            public:
                void start(Params p);
                void loop();

            };
        }
    }
}

#endif
#ifndef __RFIR_MODULE_WIFI_CLIENT_H__
#define __RFIR_MODULE_WIFI_CLIENT_H__

#include <string>

namespace network {
    namespace module {
        namespace wifi {
            class Client {
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

#endif //__RFIR_MODULE_WIFI_CLIENT_H__
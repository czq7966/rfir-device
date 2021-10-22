#ifndef __NETWORK_MODULE_WIFI_CLIENT_H__
#define __NETWORK_MODULE_WIFI_CLIENT_H__

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
                void*   manager;
                Params  params;
            public:
                void start(Params p);
                void loop();
                void checkOrReset(long timeout = 60);

            };
        }
    }
}

#endif
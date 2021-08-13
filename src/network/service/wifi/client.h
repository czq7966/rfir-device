#ifndef __RFIR_SERVICE_WIFI_CLIENT_H__
#define __RFIR_SERVICE_WIFI_CLIENT_H__


#include "network/module/wifi/client.h"

namespace network {
    namespace service {
        namespace wifi {
            class Client {
            private:
                static network::module::wifi::Client* client;

            public:
                static void Start(network::module::wifi::Client::Params p);
                static void Loop();

            };
        }
    }
}

#endif
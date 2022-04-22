#ifndef _NETWORK_MODULE_NET_NETWORKING_H_
#define _NETWORK_MODULE_NET_NETWORKING_H_
#include "../mqtt/client.h"

namespace network {
    namespace module {
        namespace net {
            class Networking {
            public:
                bool logined = false;
                network::module::mqtt::Client* mqtt;

            public:
                void start(network::module::mqtt::Client* mqtt);
                void loop();
                void login();
            public:
                static void* OnConnected(void* arg, void* p);
                static void* OnLoginResp(void* arg, void* p);
            };

        }
    }
}



#endif

#ifndef __NETWORK_SERVICE_MQTT_CLIENT_H__
#define __NETWORK_SERVICE_MQTT_CLIENT_H__

#include "rfir/util/platform.h"
#include "network/module/mqtt/async-client.h"

namespace network {
    namespace service {
        namespace mqtt {
            class AClient {
            public:
                // static network::module::mqtt::AClient* client;
            public:
                static void Start(network::module::mqtt::AClient::Params p);
                static void Loop();
            };

        }
    }
}



#endif //__RFIR_SERVICE_MQTT_CLIENT_H__

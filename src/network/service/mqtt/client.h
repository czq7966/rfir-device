
#ifndef __RFIR_SERVICE_MQTT_CLIENT_H__
#define __RFIR_SERVICE_MQTT_CLIENT_H__

#include "rfir/util/platform.h"
#include "network/module/mqtt/client.h"

namespace network {
    namespace service {
        namespace mqtt {
            class Client {
            public:
                static network::module::mqtt::Client* client;
            public:
                static void Start(network::module::mqtt::Client::Params p);
                static void Loop();
                static bool Publish(const char* payload);
                static void OnMessage(MQTTClient *client, char topic[], char bytes[], int length);
            };

        }
    }
}



#endif //__RFIR_SERVICE_MQTT_CLIENT_H__

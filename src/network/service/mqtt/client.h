
#ifndef __NETWORK_SERVICE_MQTT_CLIENT_H__
#define __NETWORK_SERVICE_MQTT_CLIENT_H__

#include "rfir/util/platform.h"
#include "network/module/mqtt/client.h"

namespace network {
    namespace service {
        namespace mqtt {
            class Client {
            public:
                static network::module::mqtt::Client* client;
            public:
                static void Start(  network::module::mqtt::Client::Params p,
                                    network::module::mqtt::Client::OnConnectEvent onConnect = 0,
                                    network::module::mqtt::Client::OnMessageEvent onMessage = 0);
                static void Loop();
                static bool Publish(const char* payload);
                static void OnMessage(MQTTClient *client, char topic[], char bytes[], int length);
            };

        }
    }
}



#endif //__RFIR_SERVICE_MQTT_CLIENT_H__

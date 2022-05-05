#include "async-client.h"
#include "rfir/service/cmds/cmd.h"

// network::module::mqtt::AClient* network::service::mqtt::AClient::client = 0;

void network::service::mqtt::AClient::Start(network::module::mqtt::AClient::Params p) {
    // if (!client) 
    //     client = new network::module::mqtt::AClient();
    // client->init(p);
    // client->start();
    GMqttClient.start(p);
    // GMqttClient.init(p);
}

void network::service::mqtt::AClient::Loop() {
    GMqttClient.loop();
    // if (client)
    //     client->loop();
}

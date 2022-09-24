#include "async-client.h"


void network::service::mqtt::AClient::Start(network::module::mqtt::AClient::Params p) {
    GMqttClient.start(p);
}

void network::service::mqtt::AClient::Loop() {
    GMqttClient.loop();
}

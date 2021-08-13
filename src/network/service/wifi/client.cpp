#include "client.h"

network::module::wifi::Client* network::service::wifi::Client::client = 0;

void network::service::wifi::Client::Start(network::module::wifi::Client::Params p) {
    if (!client)
        client = new network::module::wifi::Client();
    client->start(p);
}


void network::service::wifi::Client::Loop() {
    if (client)
        client->loop();
}
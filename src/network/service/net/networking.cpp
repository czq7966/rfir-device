#include "networking.h"

network::module::net::Networking* network::service::net::Networking::networking = 0;

void network::service::net::Networking::Start(network::module::mqtt::Client* p) {
    if (!networking)
        networking = new network::module::net::Networking();
    networking->start(p);
}


void network::service::net::Networking::Loop() {
    networking->loop();
}

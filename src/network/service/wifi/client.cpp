#include "client.h"
#include "ap.h"

network::module::wifi::Client* network::service::wifi::Client::client = 0;

void network::service::wifi::Client::Start(network::module::wifi::Client::Params p) {
    if (!client)
        client = new network::module::wifi::Client();
    client->start(p);
    if (client->params.apMode)
        AP::Start(p.ap);
}


void network::service::wifi::Client::Loop() {
    if (client) {
        client->loop();
        if (client->params.apMode)
            AP::Loop();
    }
}
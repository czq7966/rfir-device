#include "client.h"
#include "ap.h"
#include "smc.h"

network::module::wifi::Client* network::service::wifi::Client::client = 0;

void network::service::wifi::Client::Start(network::module::wifi::Client::Params& p) {
    if (!client)
        client = new network::module::wifi::Client();
    client->start(p);
    if (client->params.apMode)
        AP::Start(client->params.ap);
    if (client->params.smcMode) {        
        SMC::Start(&(client->params.smc));
    }
}


void network::service::wifi::Client::Loop() {
    if (client) {
        if (client->params.apMode)
            AP::Loop();
        if (client->params.smcMode)
            SMC::Loop();            
        client->loop();            
    }
}
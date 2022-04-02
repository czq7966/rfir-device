#include "smc.h"

network::module::wifi::SMC* network::service::wifi::SMC::smc = 0;

void network::service::wifi::SMC::Start(module::wifi::SMC::Params* p) {
    if (!smc)
        smc = new network::module::wifi::SMC();
    smc->start(p);
}


void network::service::wifi::SMC::Loop() {
    if (smc)
        smc->loop();
}
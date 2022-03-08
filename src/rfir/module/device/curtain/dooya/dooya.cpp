#include "dooya.h"


void rfir::module::device::curtain::Dooya::start(void * p) {
    Device::start(p);
}

void rfir::module::device::curtain::Dooya::loop() {
    Device::loop();
}

rfir::module::ttl::Config::Device* rfir::module::device::curtain::Dooya::init() {
   this->name = "Dooya";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    d->packet.sniff.params.bufSize = 8;

    return d;   
}

bool rfir::module::device::curtain::Dooya::onCmd_set(neb::CJsonObject* pld) {
    return 0;

}

bool rfir::module::device::curtain::Dooya::onCmd_get(neb::CJsonObject* pld) {
    return 0;
}
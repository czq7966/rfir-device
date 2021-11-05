#include "fz-dm4-tearoom-gree.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::FZDM4_TeaRoom_Gree::init() {
    auto d = Kelvinator::init();
    if (!d) return 0;
#ifdef ESP32
    //采码参数
    d->packet.sniff.params.pin = 22;
    d->packet.sniff.params.bufSize = 512;
    //发码参数 
    d->packet.send.params.pin = 15;
#endif    

    return d;
}


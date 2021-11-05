#include "cl-djroom-mcquay.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_DJROOM_Mcquay::init() {
    auto d = Mcquay::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    d->packet.sniff.params.bufSize = 512;
    //发码参数 
    d->packet.send.params.pin = 4;
#endif    

    return d;
}


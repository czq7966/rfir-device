#include "cl-vrhall-f2-coolix.h"
#include "../../../../service/device/device.h"
#include "global.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_F2_Coolix::init() {
    auto d = Coolix::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    d->packet.sniff.params.bufSize = 256 + 128;
    //发码参数 
    d->packet.send.params.pin = 4;
    d->packet.send.params.repeat = 1;
#else     
    //采码参数
    d->packet.sniff.params.pin = 15;
    // 发码参数 
    d->packet.send.params.pin = 22;  
#endif    

    return d;
}


void rfir::module::device::ac::CL_VRHALL_F2_Coolix::start(void * p) {
    Coolix::start(p);

}


void rfir::module::device::ac::CL_VRHALL_F2_Coolix::loop() {
    Coolix::loop();
}


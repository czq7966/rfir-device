#include "esp32_wroom.h"


rfir::module::ttl::Config::Device* rfir::module::device::test::Sniffer_esp32_wroom::init() {
    auto d = Sniffer::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    // 发码参数 
    d->packet.send.params.pin = 4;
#else
    //采码参数
    d->packet.sniff.params.pin = 15;
    d->packet.sniff.params.minCount = 32;
    d->packet.sniff.params.minDelta = 150;    
    // 发码参数 
    d->packet.send.params.pin = 0;        
#endif    

    return d;
}


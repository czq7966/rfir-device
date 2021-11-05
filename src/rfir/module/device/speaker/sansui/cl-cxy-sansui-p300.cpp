#include "cl-cxy-sansui-p300.h"


rfir::module::ttl::Config::Device* rfir::module::device::speaker::CL_CXY_SANSUI_P300::init() {
    auto d = SANSUI::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    // 发码参数 
    d->packet.send.params.pin = 4;
#endif    

    return d;
}


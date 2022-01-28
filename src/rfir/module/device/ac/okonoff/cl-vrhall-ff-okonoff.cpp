#include "cl-vrhall-ff-okonoff.h"
#include "../../../sensor/hlw8110/hlw8110.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_FF_Okonoff::init() {
    auto d = Okonoff::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    d->packet.sniff.params.bufSize = 512;
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


void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::start(void * p) {
    Okonoff::start(p);
    sensor::HLW8110::Start();
    sensor::HLW8110::On_IA_Switch = On_HLW8110_IA_Switch;    
    sensor::HLW8110::IA_Switch_CB_Arg = (int)this;
}


void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::loop() {
    Okonoff::loop();
    sensor::HLW8110::Loop();
}

bool rfir::module::device::ac::CL_VRHALL_FF_Okonoff::onCmd_get(neb::CJsonObject* pld) {
    Okonoff::onCmd_get(pld);
    auto iaValue = sensor::HLW8110::Hlw8110_Get_Current();
    auto running = iaValue >= sensor::HLW8110::IA_Switch_Point;
    std::string runningStr = running ? "on" : "off";
    pld->Add("running", runningStr);
    pld->Add("iaValue", iaValue);
    return true;
}

void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::On_HLW8110_IA_Switch(int arg, bool ison) {
    auto device = (CL_VRHALL_FF_Okonoff*)arg;
    if (device) {
        device->emitChange("On IA Switch");
    }
}
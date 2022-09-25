#include "fz-dm-midea.h"


void rfir::module::device::ac::FZ_DM_Midea::init() {
    Midea::init();
#ifdef ESP8266
    sniffer->params.pin = 14;
    sender->params.pin = 4;
#else     
    sniffer->params.pin = 15;
    sender->params.pin = 23;
#endif  
    sniffer->name = "FZ_DM_Midea";
    sniffer->params.maxCount = 256;
    sender->params.repeat = 1;
}


void rfir::module::device::ac::FZ_DM_Midea::start(void * p) {
    Midea::start(p);
    pinMode(PIN_POWER, INPUT);
}

int rfir::module::device::ac::FZ_DM_Midea::onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    auto r = Midea::onSvc_get(pld, cmd);
    auto running = digitalRead(PIN_POWER) ? "on" : "off";
    (*pld)["running"] = running;    
    (*pld)["power"] = running;    
    return r;
};



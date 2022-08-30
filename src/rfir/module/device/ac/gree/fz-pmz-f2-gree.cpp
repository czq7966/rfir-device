#include "fz-pmz-f2-gree.h"

void rfir::module::device::ac::FZ_PMZ_F2_Gree::init() {
    Gree::init();

#ifdef ESP8266
    sniffer->params.pin = 14;
    sender->params.pin = 4;
#else     
    sniffer->params.pin = 15;
    sender->params.pin = 23;
#endif  
    sniffer->name = "FZ_PMZ_F2_Gree";
    sniffer->params.maxCount = 140;
    sender->params.repeat = 1;
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::start(void * p) {
    Gree::start(p);
    pinMode(PIN_POWER, INPUT);
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::loop() {
    Gree::loop();
    //todo
}

int rfir::module::device::ac::FZ_PMZ_F2_Gree::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    auto r = Gree::onSvc_get(pld, cmd);

    int on = 0;
    auto time = millis();
    while (millis() - time < 300)
    {
        if (digitalRead(PIN_POWER)) {
            on++;
            break;;
        }
    }
    
    

    // auto running = digitalRead(PIN_POWER) ? "on" : "off";
    auto running = on ? "on" : "off";
    pld->ReplaceAdd("running", running);    
    pld->ReplaceAdd("power", running);    
    return r;
};

int rfir::module::device::ac::FZ_PMZ_F2_Gree::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    ac.ac->setLight(true);
    auto r = Gree::onSvc_set(pld, cmd);    
    return r;
}

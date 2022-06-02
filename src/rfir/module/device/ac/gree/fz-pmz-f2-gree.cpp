#include "fz-pmz-f2-gree.h"
#include "../../../../service/device/device.h"
#include "global.h"


bool rfir::module::device::ac::FZ_PMZ_F2_Gree::PowerPinChanged = false;

rfir::module::ttl::Config::Device* rfir::module::device::ac::FZ_PMZ_F2_Gree::init() {
    Gree::init();

#ifdef ESP8266
    sniffer->params.pin = 14;
    sender->params.pin = 4;
#else     
    sniffer->params.pin = 15;
    sender->params.pin = 23;
#endif  
    sniffer->name = this->name;
    sniffer->params.maxCount = 140;
    sender->params.repeat = 1;
    return 0;
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::start(void * p) {
    Gree::start(p);
    gpioPower.init(PIN_POWER, INPUT);
    gpioPower.onChange = OnPowerPinChange;
    gpioPower.start();
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::loop() {
    Gree::loop();
    //todo
}

bool rfir::module::device::ac::FZ_PMZ_F2_Gree::onCmd_get(neb::CJsonObject* pld) {
    auto r = Gree::onCmd_get(pld);
    auto running = this->gpioPower.read() ? "on" : "off";
    pld->ReplaceAdd("running", running);    
    // pld->ReplaceAdd("power", running);    
    return r;
};

bool rfir::module::device::ac::FZ_PMZ_F2_Gree::onCmd_set(neb::CJsonObject* pld) {
    ac.ac->setLight(true);
    auto r = Gree::onCmd_set(pld);
    // ac.ac->begin();
    // ac.ac->send(1);
    return r;
}
bool rfir::module::device::ac::FZ_PMZ_F2_Gree::doPowerPinChange() {
    if (PowerPinChanged) {
        PowerPinChanged = false;        
        emitChange("Power state change");        
        return true;
    }

    return false;


};

void rfir::module::device::ac::FZ_PMZ_F2_Gree::OnPowerPinChange(rfir::module::ttl::Gpio* gpio, int value) {
    PowerPinChanged = true;
};
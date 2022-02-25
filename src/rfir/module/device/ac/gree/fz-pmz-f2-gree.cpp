#include "fz-pmz-f2-gree.h"
#include "../../../../service/device/device.h"
#include "global.h"


bool rfir::module::device::ac::FZ_PMZ_F2_Gree::PowerPinChanged = false;

rfir::module::ttl::Config::Device* rfir::module::device::ac::FZ_PMZ_F2_Gree::init() {
    auto d = Gree::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    //发码参数 
    d->packet.send.params.pin = 4;
#else     
    //采码参数
    d->packet.sniff.params.pin = 15;
    // 发码参数 
    d->packet.send.params.pin = 22;  
#endif    
    d->packet.sniff.params.bufSize = 256 + 128;
    d->packet.send.params.repeat = 1;
    return d;
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::start(void * p) {
    Gree::start(p);
    gpioPower.init(PIN_POWER, INPUT);
    gpioPower.onChange = OnPowerPinChange;
    gpioPower.start();
}


void rfir::module::device::ac::FZ_PMZ_F2_Gree::loop() {
    Gree::loop();
}

bool rfir::module::device::ac::FZ_PMZ_F2_Gree::onCmd_get(neb::CJsonObject* pld) {
    auto r = Gree::onCmd_get(pld);
    pld->ReplaceAdd("power", this->gpioPower.read() ? "on" : "off");    
    return true;
};

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
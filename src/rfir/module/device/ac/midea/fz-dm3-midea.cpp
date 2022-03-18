#include "fz-dm3-midea.h"
#include "../../../../service/device/device.h"
#include "global.h"


bool rfir::module::device::ac::FZ_DM3_Midea::PowerPinChanged = false;

rfir::module::ttl::Config::Device* rfir::module::device::ac::FZ_DM3_Midea::init() {
    auto d = Midea::init();
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
    d->packet.send.params.pin = 23;  
#endif    
    d->packet.sniff.params.bufSize = 140;
    d->packet.send.params.repeat = 0;

    if (this->ac.ac)
        delete this->ac.ac;
    this->ac.ac = new IRMideaAC(d->packet.send.params.pin);

    return d;
}


void rfir::module::device::ac::FZ_DM3_Midea::start(void * p) {
    Midea::start(p);
    gpioPower.init(PIN_POWER, INPUT);
    gpioPower.onChange = OnPowerPinChange;
    gpioPower.start();
}


void rfir::module::device::ac::FZ_DM3_Midea::loop() {
    static unsigned long loop_time = millis();
    Midea::loop();
    
    if (millis() - loop_time > 3 * 1000 ) {
        this->ac.ac->setPower(!this->ac.ac->getPower());
        this->ac.ac->begin();
        this->ac.ac->send();
        loop_time = millis();
    } 
}

bool rfir::module::device::ac::FZ_DM3_Midea::onCmd_get(neb::CJsonObject* pld) {
    auto r = Midea::onCmd_get(pld);
    auto running = this->gpioPower.read() ? "on" : "off";
    pld->ReplaceAdd("running", running);    
    // pld->ReplaceAdd("power", running);    
    return r;
};

bool rfir::module::device::ac::FZ_DM3_Midea::onCmd_set(neb::CJsonObject* pld) {
    auto r = Midea::onCmd_set(pld);
    ac.ac->begin();
    ac.ac->send();
    return r;
}
bool rfir::module::device::ac::FZ_DM3_Midea::doPowerPinChange() {
    if (PowerPinChanged) {
        PowerPinChanged = false;        
        emitChange("Power state change");        
        return true;
    }

    return false;


};

void rfir::module::device::ac::FZ_DM3_Midea::OnPowerPinChange(rfir::module::ttl::Gpio* gpio, int value) {
    PowerPinChanged = true;
};
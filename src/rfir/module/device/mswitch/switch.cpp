#include "switch.h"


void* rfir::module::device::mswitch::Switch::ChangedSwitch = 0;
const std::string rfir::module::device::mswitch::Switch::SWITCH_ON = "on";
const std::string rfir::module::device::mswitch::Switch::SWITCH_OFF = "off";
const std::string rfir::module::device::mswitch::Switch::SWITCH_TOGGLE = "toggle";

rfir::module::device::mswitch::Switch::Switch() {
    this->params.name = "state";
    this->params.enabled = false;
    this->params.pinIn = -1;
    this->params.pinOut = -1;
};

bool rfir::module::device::mswitch::Switch::getState() {
    if (params.enabled)
        return gpioIn.read();
    return false;
}

std::string rfir::module::device::mswitch::Switch::getStateStr() {
    return getState() ? SWITCH_ON : SWITCH_OFF;

}

void rfir::module::device::mswitch::Switch::setState(bool state) {
    if (params.enabled)
        if (state) gpioOut.on(); else gpioOut.off();
}

void rfir::module::device::mswitch::Switch::start(void * p) {    
    if (params.enabled) {
        if (params.pinIn >=0) {
            gpioIn.setArg((void*)this);
            gpioIn.init(params.pinIn, INPUT);
            gpioIn.onChange = OnPinChange;
            gpioIn.start();
        }

        if (params.pinOut >=0) {
            gpioOut.init(params.pinOut, OUTPUT);
            setState(params.default_v);
        }
    }
};

void rfir::module::device::mswitch::Switch::loop() {

}; 

bool rfir::module::device::mswitch::Switch::onCmd_set(neb::CJsonObject* pld) {
    std::string value;
    if (pld->Get(this->params.name.c_str(), value)) {        
        String v = value.c_str();
        v.toLowerCase();
        value = v.c_str();
        if (value == SWITCH_ON)
            setState(1);
        if (value == SWITCH_OFF)
            setState(0);
        if (value == SWITCH_TOGGLE)
            setState(!getState());            
    }
    return false;
};

bool rfir::module::device::mswitch::Switch::onCmd_get(neb::CJsonObject* pld) {
    auto res =  pld->Add(this->params.name.c_str(), getState() ? SWITCH_ON : SWITCH_OFF);    
    Serial.println(pld->ToString().c_str());
    return res;
};


                    
void  rfir::module::device::mswitch::Switch::OnPinChange(rfir::module::ttl::Gpio* gpio, int value) {
    ChangedSwitch = gpio->getArg();
}                    
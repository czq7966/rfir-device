#include "led.h"

void rfir::util::Led::start(JLed* pled){
    stop();
    led = pled;    
};

void rfir::util::Led::loop(){
    update();    
};

bool rfir::util::Led::idle(){
    return led == nullptr;    
};

void rfir::util::Led::update(){
    if (led && led->IsRunning()) {
        led->Update();
    }    
};

void rfir::util::Led::stop(){
    if (led) {
        led->Stop();
        led = nullptr;
    }    
};

rfir::util::Led GLed;

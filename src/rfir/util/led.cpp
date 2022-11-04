#include "led.h"

void rfir::util::Led::start(JLed* pled){
    stop();
    led = pled;    
};

void rfir::util::Led::start(JLed* pled, void* owner){
    stop();    
    JLed* nled = (JLed*)this->events.onStart.emit(owner);
    this->led = pled ? pled : nled;    
    this->owner = owner;
    if (this->led)
        this->led->Reset();
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

void rfir::util::Led::stop(void* owner){
    if (owner == this->owner) {
        if (led) {
            led->Stop();
            led = nullptr;
        }    
        this->owner = 0;
    }
};


rfir::util::Led GLed;

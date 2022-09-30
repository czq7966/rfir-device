#include "button.h"
#include "interrupt.h"

rfir::util::Button::Button(){
    this->events.onClick.setCheck(std::bind(&Button::onClickCheck, this, std::placeholders::_1, std::placeholders::_2));
    this->events.onLongClick.setCheck(std::bind(&Button::onLongClickCheck, this, std::placeholders::_1, std::placeholders::_2));
    this->events.onLongPressed.setCheck(std::bind(&Button::onLongPressedCheck, this, std::placeholders::_1, std::placeholders::_2));
    this->events.onLongReleased.setCheck(std::bind(&Button::onLongReleasedCheck, this, std::placeholders::_1, std::placeholders::_2));
    this->events.onBetweenReleased.setCheck(std::bind(&Button::onBetweenReleasedCheck, this, std::placeholders::_1, std::placeholders::_2));
};

void* rfir::util::Button::onClickCheck(void* arg, void* p){
    return this->onBetweenReleasedCheck(arg, p);
};

void* rfir::util::Button::onLongClickCheck(void* arg, void* p) {
    return this->onBetweenReleasedCheck(arg, p);
};

void* rfir::util::Button::onLongPressedCheck(void* arg, void* p){
    if (arg && p) {
        auto check = (KeyTime*)arg;
        auto value = (KeyTime*)p;
        if (value->pressed > check->pressed) {
            return (void*)1;
        }
    }
    return 0;
};

void* rfir::util::Button::onLongReleasedCheck(void* arg, void* p){
    DEBUGER.println("rfir::util::Button::onBetweenReleasedCheck");
    if (arg && p) {        
        auto check = (KeyTime*)arg;
        auto value = (KeyTime*)p;
        DEBUGER.printf("rfir::util::Button::onBetweenReleasedCheck %d > %d \r\n", value->released, check->released);
        if (value->released > check->released ) {
            return (void*)1;
        }
    }
    return 0;
};

void* rfir::util::Button::onBetweenReleasedCheck(void* arg, void* p){
    DEBUGER.println("rfir::util::Button::onBetweenReleasedCheck");
    if (arg && p) {
        auto check = (KeyTime*)arg;
        auto value = (KeyTime*)p;

        DEBUGER.printf("rfir::util::Button::onBetweenReleasedCheck %d > %d, %d < %d  \r\n", value->released, check->released, value->released , check->released);

        if (value->released > check->pressed && value->released < check->released) {
            return (void*)1;
        }
    }
    return 0;
};


void rfir::util::Button::start(int pin, int pressedValue){
    stop();
    this->pin = pin;
    this->pressedValue = pressedValue;
    if (this->pin >= 0) {        
        auto pinEvent = GInterrupt.getPinEvent(this->pin);
        pinEvent->event->off(this);
        pinEvent->event->on(this, std::bind(&Button::onPinInterrupt, this, std::placeholders::_1, std::placeholders::_2), this);
    }
    start();
};

void rfir::util::Button::start(){
    if (this->pin >= 0) {
        DEBUGER.println("rfir::util::Button::start()");
        GInterrupt.start(this->pin);
    }    
};

void rfir::util::Button::stop(){
    if (this->pin >= 0) {
        GInterrupt.stop(this->pin);
    }    
};

void rfir::util::Button::loop(){
    if (this->keyTime.pressed && !this->keyTime.released) {
        KeyTime time;
        time.pressed = millis() - this->keyTime.pressed;
        this->events.onLongPressed.emit((void*)&time, true);
    }

    if (this->keyTime.released) {        
        if (this->keyTime.pressed) {
            KeyTime time;
            time.pressed = 1;
            time.released = this->keyTime.released - this->keyTime.pressed + time.pressed;
            this->events.onLongReleased.emit((void*)&time, true);
            this->events.onBetweenReleased.emit((void*)&time, true);            
            DEBUGER.println("rfir::util::Button::loop released");
        }        

        this->keyTime.pressed = 0;
        this->keyTime.released = 0;
    }    
};

void* rfir::util::Button::onPinInterrupt(void* arg, void* p) {
    auto pinEvent = (rfir::util::Interrupt::PinEvent*)p;
    auto button = (rfir::util::Button*)arg;
    if (pinEvent->value == button->pressedValue) {
        if (this->keyTime.released || !this->keyTime.pressed) {
            this->keyTime.released = 0;
            this->keyTime.pressed = millis();        
            this->events.onPressed.emit((void*)&this->keyTime);
        }
    }
    else {
        if (!this->keyTime.released) {
            this->keyTime.released = millis();
            this->events.onReleased.emit(p);        
        }
    }

    return 0;
};

rfir::util::Button GButton;

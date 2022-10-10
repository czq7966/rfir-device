#include "interrupt.h"


void rfir::util::Interrupt::start(int pin){
    stop(pin);

    std::list<PinEvent*> values;
    pinEvents.getValues(values);
    for (auto  it = values.begin(); it != values.end(); it++)
    {
        auto pinEvent = *it;
        bool started =  (pin == pinEvent->pin || (pin == -1 && pinEvent->started));
        
        if (started) {
            pinEvent->started = started;
            pinMode(pinEvent->pin, pinEvent->mode);
            attachInterruptArg(pinEvent->pin, OnPinInterrupt, (void*)pinEvent, CHANGE);
            GDebuger.printf("start interrupt pin: %d  \r\n", pinEvent->pin);            
        }
    }

};
void rfir::util::Interrupt::stop(int pin){
    std::list<PinEvent*> values;
    pinEvents.getValues(values);

    for (auto  it = values.begin(); it != values.end(); it++)
    {
        auto pinEvent = *it;        
        if (pin == -1 || pin == pinEvent->pin) {
            detachInterrupt(pinEvent->pin);
            pinEvent->started = (pin == pinEvent->pin ? false : pinEvent->started);
            GDebuger.printf("stop interrupt pin: %d  \r\n", pinEvent->pin);
        }
    }
    
};

rfir::util::Interrupt::PinEvent* rfir::util::Interrupt::getPinEvent(int pin, int mode){
    PinEvent* pinEvent;
    if (!pinEvents.get(pin, pinEvent)) {
        pinEvent = new PinEvent();
        pinEvent->arg = (void*)this;
        pinEvent->pin = pin;
        pinEvent->mode = mode;
        pinEvent->event = new Event();
        pinEvents.add(pin, pinEvent);
    }
    return pinEvent;
};

IRAM_ATTR void  rfir::util::Interrupt::OnPinInterrupt(void* arg) {
    auto pinEvent = (PinEvent*)arg;
    pinEvent->value = digitalRead(pinEvent->pin);

    pinEvent->event->emit((void*)pinEvent);
};

rfir::util::Interrupt GInterrupt;    

             
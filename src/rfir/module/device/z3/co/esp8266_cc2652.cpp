#include "esp8266_cc2652.h"


void rfir::module::device::z3::co::ESP8266_CC2652::init() {
    Coordinator::init();
    this->name = "Co_ESP8266_CC2652";
}


void rfir::module::device::z3::co::ESP8266_CC2652::start(void * p) {
    Coordinator::start(p);
    int pin = 15;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);

    pin = 5;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
}

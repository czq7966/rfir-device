#include <future>
#include "gpio.h"
#include "sniffer.h"
#include "rfir/service/serial/sender.h"
#include "rfir/module/cmds/cmd.h"


rfir::module::ttl::Gpio* rfir::service::ttl::Gpio::gpio = new rfir::module::ttl::Gpio();

void rfir::service::ttl::Gpio::init(uint8_t pin, uint8_t mode, bool inverted) {
    gpio->init(pin, mode, inverted);
}

void rfir::service::ttl::Gpio::start() {
    attachInterrupt(gpio->pin, gpio_interrupt, CHANGE);
}

void rfir::service::ttl::Gpio::stop() {
    detachInterrupt(gpio->pin);
}

void rfir::service::ttl::Gpio::gpio_interrupt() {
    int value = gpio->read();
    rfir::service::ttl::Sniffer::ttlsniffer->pushTTL(value);
}



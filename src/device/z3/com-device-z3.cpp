#include "com-device-z3.h"


void device::ComDeviceZ3::start() {
    ComDevice::start();
    int pin = 5;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);    
}

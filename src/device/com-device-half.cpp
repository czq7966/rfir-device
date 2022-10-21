#include "com-device-half.h"
#include "cmds/cmd/reg-table.h"

void device::ComDeviceHalf::readMode(){
    auto pin = GRegTable.tables.get(GRegTable.keys.serial_half_rw_pin);
    auto level = GRegTable.tables.get(GRegTable.keys.serial_half_r_level);
    pinMode(pin, OUTPUT);   
    digitalWrite(pin, level);
};

void device::ComDeviceHalf::writeMode(){
    auto pin = GRegTable.tables.get(GRegTable.keys.serial_half_rw_pin);
    auto level = GRegTable.tables.get(GRegTable.keys.serial_half_r_level);
    pinMode(pin, OUTPUT);   
    digitalWrite(pin, !level);
};

void device::ComDeviceHalf::start(){
    ComDevice::start();
    this->readMode();    
};

size_t device::ComDeviceHalf::write(const char* buf, const size_t size) {
    this->serial->flush();
    writeMode();
    size_t len = ComDevice::write(buf, size);
    this->serial->flush();
    readMode();      
    return len;
};

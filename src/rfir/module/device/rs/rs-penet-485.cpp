#include "rs-penet-485.h"
#include "rfir/util/base64.h"
#include "../networking.h"

void rfir::module::device::RS::RSPenet485::init() {
    RS::RSPenet::init();
    initDerePin();
    this->name = "RSPenet485";
}

void rfir::module::device::RS::RSPenet485::initDerePin() {
    this->dePin = 5;
    this->rePin = 5;    
    pinMode(dePin, OUTPUT);
    pinMode(rePin, OUTPUT);   
    readMode();   
}

void rfir::module::device::RS::RSPenet485::readMode(){
    digitalWrite(dePin, 0);
    digitalWrite(rePin, 0);
};

void rfir::module::device::RS::RSPenet485::writeMode(){
    digitalWrite(dePin, 1);
    digitalWrite(rePin, 1);
};


bool rfir::module::device::RS::RSPenet485::sendCode(uint8_t* tx_buf, int len) {
    bool result = false;
    this->hwSerial->flush();    
    writeMode();    

    result = RS::RSPenet::rsWrite((char*)tx_buf, len, 0);
    this->hwSerial->flush();

    readMode();   
    return result;
}

bool rfir::module::device::RS::RSPenet485::recvCode(uint8_t* rx_buf, int& len, unsigned long timeout_ms) {
    bool result = false;
    readMode();

    int c = -1;
    unsigned long startTime = millis();
    unsigned long entTime = startTime;

    int offset = 0;
    while (hwSerial->available() || (entTime - startTime < timeout_ms)) {
        if (hwSerial->available()) {
            int size = RS::RSPenet::rsRead((char*)rx_buf, offset); 
            if (size > 0) {
                startTime = millis();
            }

            offset += size;
        }
        entTime = millis();
    }


    len = offset;
    result = (len > 0) && checkSumCode(rx_buf, len);

    return result;
}


bool rfir::module::device::RS::RSPenet485::sendCodeAndRecv(uint8_t* tx_buf, int tx_len, uint8_t* rx_buf, int& rx_len, unsigned long timeout_ms) {
    if (sendCode(tx_buf, tx_len)) {
        return recvCode(rx_buf, rx_len, timeout_ms);
    }

    return 0;
}

bool rfir::module::device::RS::RSPenet485::checkSumCode(uint8_t* rx_buf, uint8_t len) {
    return true;
}; 

int rfir::module::device::RS::RSPenet485::rsRead(char buffer[], int offset) {
    int len = 0;
    bool result = recvCode((uint8_t*) buffer, len, recvTimeout);
    return result ? len : 0;
}

bool rfir::module::device::RS::RSPenet485::rsWrite(char buffer[], size_t length, int offset) {
    return sendCode((uint8_t*)(buffer + offset), length);
}


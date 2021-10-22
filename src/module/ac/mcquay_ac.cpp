#include "mcquay_ac.h"

void module::ac::McquayAC::setTemp(const uint8_t temp, const bool fahrenheit) {
    uint8_t safecelsius = temp;
    safecelsius = std::max(KMcQuayMinTempC, safecelsius);
    safecelsius = std::min(KMcQuayMaxTempC, safecelsius);

    protocol.Temp = dec2hex(safecelsius);    
    
}

uint8_t module::ac::McquayAC::getTemp()  {
    return hex2dec(protocol.Temp); 
}

void module::ac::McquayAC::setFan(const uint8_t speed) {
  uint8_t fan = speed;  
  protocol.Fan = fan;       
}

uint8_t module::ac::McquayAC::getFan()  {
    return protocol.Fan;
}

void    module::ac::McquayAC::setMode(const uint8_t new_mode) {
    uint8_t mode = new_mode;
    protocol.Mode = mode;
}

uint8_t module::ac::McquayAC::getMode() {
    return protocol.Mode;
}

void    module::ac::McquayAC::setSleep(const bool on) {
    protocol.Sleep = on;
}

bool    module::ac::McquayAC::getSleep() {
    return protocol.Sleep;
}

void    module::ac::McquayAC::setSwing(const bool on) {
    protocol.Swing = on;
}

bool    module::ac::McquayAC::getSwing() {
    return protocol.Swing;
}

uint8_t  module::ac::McquayAC::getTimerHour() {
    return hex2dec(protocol.TimerHour);
}

void     module::ac::McquayAC::setTimerHour(const uint8_t hour) {
    protocol.TimerHour = dec2hex(hour);
}

uint8_t  module::ac::McquayAC::getTimerMinute() {
    return hex2dec(protocol.TimerMinute);
}

void     module::ac::McquayAC::setTimerMinute(const uint8_t minute) {
    protocol.TimerHour = dec2hex(minute);
}


uint8_t* module::ac::McquayAC::getRaw(void) {
    fixup(); 
    return protocol.remote_state;
}

void    module::ac::McquayAC::setRaw(const uint8_t new_code[]) {
    std::memcpy(protocol.remote_state, new_code, KMcQuayStateLength);
}

void module::ac::McquayAC::checksum(const uint16_t length) {
    protocol.Sum = McquayAC::calcBlockChecksum(protocol.remote_state, length);
}
bool module::ac::McquayAC::validsum() {
    return validChecksum(this->protocol.remote_state);
}

void module::ac::McquayAC::fixup() {
    checksum();
}

std::string module::ac::McquayAC::toString() {
    return "";
}

std::string module::ac::McquayAC::toBitString() {
    String result;

    for (size_t j = 0; j < KMcQuayStateLength; j++)
    {
        uint8_t b = this->protocol.remote_state[j];  
        String bitStr;
        for (size_t k = 0; k < 8; k++)
        {
            bitStr = String(b & 1 ? 1: 0) + bitStr;
            b >>= 1;    
        }   

        if (result.length() > 0)
            result = result + (" " + bitStr);
        else result = result + bitStr; 
    }        

    return std::string(result.c_str());    
}

std::string module::ac::McquayAC::toHexString() {
    String result;
    for (size_t j = 0; j < KMcQuayStateLength; j++)
    {
        char c[3];
        uint8_t b = this->protocol.remote_state[j];  
        itoa(b, c, 16);
        String hex = (strlen(c) == 1) ? ("0" + String(c)) :  String(c);

        if (result.length() > 0)
            result = result + (" " + hex);
        else
            result = result + hex; 
    }
    return std::string(result.c_str());
}

uint8_t module::ac::McquayAC::calcBlockChecksum(const uint8_t* block, const uint16_t length) {
    uint8_t sum = 0;
    // Sum the upper half and lower half of this block.
    for (uint8_t i = 0; i < length - 1; i++, block++) {
        sum += (*block & 0b1111) + (*block >> 4);
    }
    sum = sum + (*block & 0b1111);

    return sum & 0b1111; 
}

bool module::ac::McquayAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return GETBITS8(state[length - 1], 4, 4) ==  calcBlockChecksum(state, length);
}

uint8_t module::ac::McquayAC::dec2hex(const uint8_t dec) {
    return  ((dec / 10) << 4) + ((dec % 10) & 0b1111);
}

uint8_t module::ac::McquayAC::hex2dec(const uint8_t hex) {
    return  (hex >> 4) * 10 + (hex & 0b1111);
}
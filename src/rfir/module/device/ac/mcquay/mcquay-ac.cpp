#include "mcquay-ac.h"

void rfir::module::device::ac::McquayAC::setHeader(const uint8_t header) {
    protocol.Header = header;
}

uint8_t rfir::module::device::ac::McquayAC::getHeader() {
    return protocol.Header;
}

void rfir::module::device::ac::McquayAC::setTemp(const uint8_t temp, const bool fahrenheit) {
    uint8_t safecelsius = (temp >= KMcQuayMinTempC && temp <= KMcQuayMaxTempC) ? temp : 25;
    protocol.Temp = dec2hex(safecelsius);        
}

uint8_t rfir::module::device::ac::McquayAC::getTemp()  {
    return hex2dec(protocol.Temp); 
}

void rfir::module::device::ac::McquayAC::setFan(const uint8_t speed) {
  uint8_t fan = speed ? speed : KMcQuayFanAuto;
  protocol.Fan = fan;       
}

uint8_t rfir::module::device::ac::McquayAC::getFan()  {
    return protocol.Fan;
}

void    rfir::module::device::ac::McquayAC::setMode(const uint8_t new_mode) {
    uint8_t mode = new_mode ? new_mode : KMcQuayModeDry;
    protocol.Mode = mode;
}

uint8_t rfir::module::device::ac::McquayAC::getMode() {
    return protocol.Mode;
}

void    rfir::module::device::ac::McquayAC::setSleep(const bool on) {
    protocol.Sleep = on;
}

bool    rfir::module::device::ac::McquayAC::getSleep() {
    return protocol.Sleep;
}

void    rfir::module::device::ac::McquayAC::setSwing(const bool on) {
    protocol.Swing = on;
}

bool    rfir::module::device::ac::McquayAC::getSwing() {
    return protocol.Swing;
}

void    rfir::module::device::ac::McquayAC::setPowerSwitch(const bool on) {
    protocol.PowerSwitch = on;
}

bool    rfir::module::device::ac::McquayAC::getPowerSwitch() {
    return protocol.PowerSwitch;
}

uint8_t  rfir::module::device::ac::McquayAC::getHour() {
    return hex2dec(protocol.Hour);
}

void     rfir::module::device::ac::McquayAC::setHour(const uint8_t hour) {
    protocol.Hour = dec2hex(hour);
}

uint8_t  rfir::module::device::ac::McquayAC::getMinute() {
    return hex2dec(protocol.Minute);
}

void     rfir::module::device::ac::McquayAC::setMinute(const uint8_t minute) {
    protocol.Minute = dec2hex(minute);
}


uint8_t* rfir::module::device::ac::McquayAC::getRaw(void) {
    fixup(); 
    return protocol.remote_state;
}

void    rfir::module::device::ac::McquayAC::setRaw(const uint8_t new_code[]) {
    memcpy(protocol.remote_state, new_code, KMcQuayStateLength);
    if (this->onSetRaw)
        this->onSetRaw(this);
}

void rfir::module::device::ac::McquayAC::checksum(const uint16_t length) {
    protocol.Sum = McquayAC::calcBlockChecksum(protocol.remote_state, length);
}
bool rfir::module::device::ac::McquayAC::validsum() {
    return validChecksum(this->protocol.remote_state);
}

void rfir::module::device::ac::McquayAC::fixup() {
    protocol.Header = KMcQuayHeader;
    setMode(getMode());
    setFan(getFan());
    setTemp(getTemp());
    setSleep(getSleep());
    setSwing(getSwing());
    
    checksum();
}

std::string rfir::module::device::ac::McquayAC::toString() {
    return "";
}

std::string rfir::module::device::ac::McquayAC::toBitString() {
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

std::string rfir::module::device::ac::McquayAC::toHexString() {
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

uint16_t* rfir::module::device::ac::McquayAC::getEncodeRaw() {

    uint16_t offset = 0;

    //Header
    this->encodeRaw[offset++] = KMcQuayEncodeHeaderMark;
    this->encodeRaw[offset++] = KMcQuayEncodeHeaderSpace;

    auto raw = getRaw();
    uint8_t prebit = 0;
    for(uint8_t i = 0; i < 8; i++) {
        auto byte = *(raw + i);
        for(uint8_t j = 0; j < 8; j++) {
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KMcQuayEncodeZeroMark;
                space = KMcQuayEncodeZeroSpace;
            } else {
                mark = KMcQuayEncodeOneMark;
                space = KMcQuayEncodeOneSpace;
            }

            if ( i + j == 0)  
                mark += 40;
            
            if ((j + 1) % 4 == 0)
                space += 70;
            if (prebit == 1) 
                mark += 30;

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;

            prebit = bit;
        }
    }
    this->encodeRaw[offset++] = KMcQuayEncodeFooterMark;
    this->encodeRaw[offset++] = KMcQuayEncodeFooterSpace;

    this->encodeRaw[offset++] = KMcQuayEncodeHeaderMark;
    this->encodeRaw[offset++] = KMcQuayEncodeHeaderSpace;


    return this->encodeRaw;
}

std::string rfir::module::device::ac::McquayAC::getEncodeString() {
    auto raw = getEncodeRaw();
    String result;
    for(auto i = 0; i < KMcQuayEncodeRawLength; i++) {
        result = result + String(*(raw + i)) + ",";
    }
    return std::string(result.c_str());
}

uint8_t rfir::module::device::ac::McquayAC::calcBlockChecksum(const uint8_t* block, const uint16_t length) {
    uint8_t sum = 0;
    // Sum the upper half and lower half of this block.
    for (uint8_t i = 0; i < length - 1; i++, block++) {
        sum += (*block & 0b1111) + (*block >> 4);
    }
    sum = sum + (*block & 0b1111);

    return sum & 0b1111; 
}

bool rfir::module::device::ac::McquayAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return GETBITS8(state[length - 1], 4, 4) ==  calcBlockChecksum(state, length);
}

uint8_t rfir::module::device::ac::McquayAC::dec2hex(const uint8_t dec) {
    return  ((dec / 10) << 4) + ((dec % 10) & 0b1111);
}

uint8_t rfir::module::device::ac::McquayAC::hex2dec(const uint8_t hex) {
    return  (hex >> 4) * 10 + (hex & 0b1111);
}
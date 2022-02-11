#include "coolix-ac.h"


rfir::module::device::ac::CoolixAC::CoolixAC() {
    fixup();
}

void rfir::module::device::ac::CoolixAC::setHeader() {
    protocol.H1 = 0x65;
    protocol.H2 = 0x9a;
    protocol.H3 = 0x87;
    protocol.H4 = 0x78;
}

void rfir::module::device::ac::CoolixAC::setFooter() {
    protocol.F1 = 0xFF;
    protocol.F2 = 0x00;
    protocol.F3 = 0xFF;
    protocol.F4 = 0x00;
}



void rfir::module::device::ac::CoolixAC::setTemp(const uint8_t temp, const bool fahrenheit) {
    uint8_t t = (temp >= KCoolixMinTempC && temp <= KCoolixMaxTempC) ? temp : 25;
    t = t - 5;
    uint8_t t1 = t > 15 ? 1 : 0;
    uint8_t t2 = t > 15 ? t - 15 : t;
    protocol.Temp1 = t1;
    protocol.Temp2 = t2;    
}

uint8_t rfir::module::device::ac::CoolixAC::getTemp()  {
    return protocol.Temp1 * 15 + protocol.Temp2 + 5;
}

void rfir::module::device::ac::CoolixAC::setFan(const uint8_t speed) {
  protocol.Fan = speed;       
}

uint8_t rfir::module::device::ac::CoolixAC::getFan()  {
    return protocol.Fan;
}

void    rfir::module::device::ac::CoolixAC::setMode(const uint8_t new_mode) {
    protocol.Mode = new_mode == KCoolixModeHeat ? KCoolixModeHeat: KCoolixModeCool;
}

uint8_t rfir::module::device::ac::CoolixAC::getMode() {
    return protocol.Mode;
}



void    rfir::module::device::ac::CoolixAC::setPower(const bool on) {
    protocol.Power = on ? KCoolixPowerOn : KCoolixPowerOff;
}

bool    rfir::module::device::ac::CoolixAC::getPower() {
    return protocol.Power == KCoolixPowerOff ? false : true;
}


uint8_t* rfir::module::device::ac::CoolixAC::getRaw(void) {
    fixup(); 
    return protocol.remote_state;
}

void    rfir::module::device::ac::CoolixAC::setRaw(const uint8_t new_code[]) {
    memcpy(protocol.remote_state, new_code, KCoolixStateLength);
    if (this->onSetRaw)
        this->onSetRaw(this);
}

void rfir::module::device::ac::CoolixAC::checksum(const uint16_t length) {

}

bool rfir::module::device::ac::CoolixAC::validsum() {
    return true;
}

void rfir::module::device::ac::CoolixAC::fixup() {

    setHeader();
    protocol.S1 = KCoolixSep;

    setPower(getPower());
    setMode(getMode());
    protocol.N1 = (protocol.Mode << 4) + protocol.Power;
    protocol.N1 = ~protocol.N1;

    setFan(getFan());
    setTemp(getTemp());
    protocol.N2 = (protocol.Temp2 << 4) + (protocol.Temp1 << 3) + protocol.Fan;
    protocol.N2 = ~protocol.N2;

    protocol.S2 = KCoolixSep;
    setFooter();



    checksum();
}

std::string rfir::module::device::ac::CoolixAC::toString() {
    return "";
}

std::string rfir::module::device::ac::CoolixAC::toBitString() {
    String result;

    for (size_t j = 0; j < KCoolixStateLength; j++)
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

std::string rfir::module::device::ac::CoolixAC::toHexString() {
    String result;
    for (size_t j = 0; j < KCoolixStateLength; j++)
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

uint16_t* rfir::module::device::ac::CoolixAC::getEncodeRaw() {

    uint16_t offset = 0;

    //Header
    this->encodeRaw[offset++] = KCoolixEncodeHeaderMark;
    this->encodeRaw[offset++] = KCoolixEncodeHeaderSpace;

    auto raw = getRaw();

    for(uint8_t i = 0; i < KCoolixStateLength; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KCoolixEncodeZeroMark;
                space = KCoolixEncodeZeroSpace;
            } else {
                mark = KCoolixEncodeOneMark;
                space = KCoolixEncodeOneSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }
    this->encodeRaw[offset++] = KCoolixEncodeFooterMark;
    this->encodeRaw[offset++] = KCoolixEncodeFooterSpace;

    uint32_t temp = 0;
    Serial.println(KCoolixEncodeRawLength);
    for (size_t i = 0; i < KCoolixEncodeRawLength; i++)
    {
        // Serial.println(this->encodeRaw[i]);
        temp += this->encodeRaw[i];        
    }

    Serial.println(temp);
    

    return this->encodeRaw;
}

std::string rfir::module::device::ac::CoolixAC::getEncodeString() {
    auto raw = getEncodeRaw();
    String result;
    for(auto i = 0; i < KCoolixEncodeRawLength; i++) {
        result = result + String(*(raw + i)) + ",";
    }
    return std::string(result.c_str());
}

uint8_t rfir::module::device::ac::CoolixAC::calcBlockChecksum(const uint8_t* block, const uint16_t length) {
    uint8_t sum = 0;
    // Sum the upper half and lower half of this block.
    for (uint8_t i = 0; i < length - 1; i++, block++) {
        sum += (*block & 0b1111) + (*block >> 4);
    }
    sum = sum + (*block & 0b1111);

    return sum & 0b1111; 
}

bool rfir::module::device::ac::CoolixAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return true;
}

uint8_t rfir::module::device::ac::CoolixAC::dec2hex(const uint8_t dec) {
    return  ((dec / 10) << 4) + ((dec % 10) & 0b1111);
}

uint8_t rfir::module::device::ac::CoolixAC::hex2dec(const uint8_t hex) {
    return  (hex >> 4) * 10 + (hex & 0b1111);
}
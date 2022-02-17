#include "coolix-ac.h"
#include "rfir/util/util.h"


rfir::module::device::ac::CoolixAC::CoolixAC() {
    this->ac = new IRCoolixAC(0);
    fixup();
}

void rfir::module::device::ac::CoolixAC::setTemp(const uint8_t temp, const bool fahrenheit) {
    this->ac->setTemp(temp); 
}

uint8_t rfir::module::device::ac::CoolixAC::getTemp()  {
    return this->ac->getTemp();
}

void rfir::module::device::ac::CoolixAC::setFan(const uint8_t speed) {
    this->ac->setFan(speed);
}

uint8_t rfir::module::device::ac::CoolixAC::getFan()  {
    return this->ac->getFan();
}

void    rfir::module::device::ac::CoolixAC::setMode(const uint8_t new_mode) {
    this->ac->setMode(new_mode);
}

uint8_t rfir::module::device::ac::CoolixAC::getMode() {
    return this->ac->getMode();
}



void    rfir::module::device::ac::CoolixAC::setPower(const bool on) {
    this->ac->setPower(on);
    
}

bool    rfir::module::device::ac::CoolixAC::getPower() {
    return this->ac->getPower();
}


uint8_t* rfir::module::device::ac::CoolixAC::getRaw(void) {
    fixup(); 
    return protocol.remote_state;
}

void    rfir::module::device::ac::CoolixAC::setRaw(const uint8_t new_code[]) {
    memcpy(protocol.remote_state, new_code, KCoolixStateLength);
    uint32_t raw0 = protocol.remote_state[0];
    uint32_t raw1 = protocol.remote_state[2];
    uint32_t raw2 = protocol.remote_state[4];
    uint32_t raw = raw0 << 16 | raw1 << 8 | raw2;
    this->ac->setRaw(raw);
    //Save state
    auto power = getPower();
    setPower(false);
    setPower(power);

    fixup();
    if (this->onSetRaw)
        this->onSetRaw(this);
}

void rfir::module::device::ac::CoolixAC::checksum(const uint16_t length) {

}

bool rfir::module::device::ac::CoolixAC::validsum() {
    return true;
}

void rfir::module::device::ac::CoolixAC::fixup() {
    uint32_t raw =  this->ac->getRaw();
    for (size_t i = 8; i <= kCoolixBits; i += 8)
    {
        int idx = i / 8;
        int pos = (idx - 1) * 2;
        protocol.remote_state[pos] = (raw >> (kCoolixBits - i)) & 0xFF;
        protocol.remote_state[pos + 1] = protocol.remote_state[pos] ^ 0xFF;
    }
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
            auto bit = byte >> (7 - j) & 0x01;
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

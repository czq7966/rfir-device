#include "midea-ac.h"
#include "rfir/util/util.h"


rfir::module::device::ac::MideaAC::MideaAC() {
    this->ac = new IRMideaAC(0);
}


std::string rfir::module::device::ac::MideaAC::toString() {
    return "";
}

std::string rfir::module::device::ac::MideaAC::toBitString() {
    String result;
    auto _raw = this->ac->getRaw();
    uint8_t* raw = (uint8_t*)&_raw;
    for (size_t j = 0; j < KMideaStateLength; j++)
    {
        uint8_t b = raw[j];  
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

std::string rfir::module::device::ac::MideaAC::toHexString() {
    String result;
    auto _raw = this->ac->getRaw();
    uint8_t* raw = (uint8_t*)&_raw;
    for (size_t j = 0; j < KMideaStateLength; j++)
    {
        char c[3];        
        uint8_t b = raw[j];  
        itoa(b, c, 16);
        String hex = (strlen(c) == 1) ? ("0" + String(c)) :  String(c);

        if (result.length() > 0)
            result = result + (" " + hex);
        else
            result = result + hex; 
    }
    return std::string(result.c_str());
}

uint16_t* rfir::module::device::ac::MideaAC::getEncodeRaw() {
    return 0;

    uint16_t offset = 0;

    //Header
    this->encodeRaw[offset++] = KMideaHdrMark;
    this->encodeRaw[offset++] = KMideaHdrSpace;

    auto _raw = this->ac->getRaw();
    uint8_t* raw = (uint8_t*)&_raw;

    //前4字节
    for(uint8_t i = 0; i < KMideaStateLength / 2; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KMideaBitMark;
                space = KMideaOneSpace;
            } else {
                mark = KMideaBitMark;
                space = KMideaZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //中间3位010 与Footer
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaZeroSpace;    
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaOneSpace;    
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaZeroSpace;     
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaMsgSpace;       

    //后4字节
    for(uint8_t i = KMideaStateLength / 2; i < KMideaStateLength ; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KMideaBitMark;
                space = KMideaOneSpace;
            } else {
                mark = KMideaBitMark;
                space = KMideaZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //Footer
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaMsgSpace * 2;

    /////再发一次前4字节及中间位
    //前4字节
    this->encodeRaw[offset++] = KMideaHdrMark;
    this->encodeRaw[offset++] = KMideaHdrSpace;
    for(uint8_t i = 0; i < KMideaStateLength / 2; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KMideaBitMark;
                space = KMideaOneSpace;
            } else {
                mark = KMideaBitMark;
                space = KMideaZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //中间3位010 与Footer
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaZeroSpace;    
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaOneSpace;    
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaZeroSpace;     
    this->encodeRaw[offset++] = KMideaBitMark;
    this->encodeRaw[offset++] = KMideaMsgSpace; 
    //////////////////////////////////////////////      

    return this->encodeRaw;
}

std::string rfir::module::device::ac::MideaAC::getEncodeString() {
    auto raw = getEncodeRaw();
    String result;
    for(auto i = 0; i < KMideaEncodeRawLength; i++) {
        result = result + String(*(raw + i)) + ",";
    }
    return std::string(result.c_str());
}

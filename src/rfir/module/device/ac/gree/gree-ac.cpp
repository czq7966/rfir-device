#include "gree-ac.h"
#include "rfir/util/util.h"


rfir::module::device::ac::GreeAC::GreeAC() {
    this->ac = new IRGreeAC(0);
}


std::string rfir::module::device::ac::GreeAC::toString() {
    return "";
}

std::string rfir::module::device::ac::GreeAC::toBitString() {
    String result;
    uint8_t* raw = this->ac->getRaw();
    for (size_t j = 0; j < kGreeStateLength; j++)
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

std::string rfir::module::device::ac::GreeAC::toHexString() {
    String result;
    uint8_t* raw = this->ac->getRaw();
    for (size_t j = 0; j < kGreeStateLength; j++)
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

uint16_t* rfir::module::device::ac::GreeAC::getEncodeRaw() {
    return 0;

    uint16_t offset = 0;

    //Header
    this->encodeRaw[offset++] = KGreeHdrMark;
    this->encodeRaw[offset++] = KGreeHdrSpace;

    auto raw = this->ac->getRaw();

    //前4字节
    for(uint8_t i = 0; i < kGreeStateLength / 2; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KGreeBitMark;
                space = KGreeOneSpace;
            } else {
                mark = KGreeBitMark;
                space = KGreeZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //中间3位010 与Footer
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeZeroSpace;    
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeOneSpace;    
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeZeroSpace;     
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeMsgSpace;       

    //后4字节
    for(uint8_t i = kGreeStateLength / 2; i < kGreeStateLength ; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KGreeBitMark;
                space = KGreeOneSpace;
            } else {
                mark = KGreeBitMark;
                space = KGreeZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //Footer
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeMsgSpace * 2;

    /////再发一次前4字节及中间位
    //前4字节
    this->encodeRaw[offset++] = KGreeHdrMark;
    this->encodeRaw[offset++] = KGreeHdrSpace;
    for(uint8_t i = 0; i < kGreeStateLength / 2; i++) {
        auto byte = *(raw + i);        
        for(uint8_t j = 0; j < 8; j++) {
            //MSBFirst==true
            // auto bit = byte >> (7 - j) & 0x01;
            //MSBFirst==false
            auto bit = byte >> j & 0x01;
            uint16_t mark = 0, space = 0;
            if (bit == 0) {
                mark = KGreeBitMark;
                space = KGreeOneSpace;
            } else {
                mark = KGreeBitMark;
                space = KGreeZeroSpace;
            }

            this->encodeRaw[offset++] = mark;
            this->encodeRaw[offset++] = space;
        }
    }

    //中间3位010 与Footer
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeZeroSpace;    
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeOneSpace;    
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeZeroSpace;     
    this->encodeRaw[offset++] = KGreeBitMark;
    this->encodeRaw[offset++] = KGreeMsgSpace; 
    //////////////////////////////////////////////      

    return this->encodeRaw;
}

std::string rfir::module::device::ac::GreeAC::getEncodeString() {
    auto raw = getEncodeRaw();
    String result;
    for(auto i = 0; i < KGreeEncodeRawLength; i++) {
        result = result + String(*(raw + i)) + ",";
    }
    return std::string(result.c_str());
}

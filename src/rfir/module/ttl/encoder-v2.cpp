#include "encoder-v2.h"


int rfir::module::ttl::EncoderV2::encodeBits(Params p, uint64_t bits, int nbits, std::list<uint16_t>& result){
    int offset = result.size();

    if (p.MSBfirst) {
        for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1) {
        if (bits & mask) {  // Send a 1
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                    result.push_back(p.onespace);
                } else { //Event Mark
                    result.push_back(p.onemark);
                }
            } else {
                result.push_back(p.onemark);
                result.push_back(p.onespace);
            }
        } else {  // Send a 0
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                    result.push_back(p.zerospace);
                } else { //Event Mark
                    result.push_back(p.zeromark);
                }              
            } else {
                result.push_back(p.zeromark);
                result.push_back(p.zerospace);
            }
        } 
    }
    } else {
        for (size_t i = 0; i < nbits; i++) {    
        if ((bits >> i) & 1) {  // Send a 1
            if (p.step == 1){
                if (offset & 2) { //Odd Space
                    result.push_back(p.onespace);
                } else { //Event Mark
                    result.push_back(p.onemark);
                }
            } else {
                result.push_back(p.onemark);
                result.push_back(p.onespace);
            }
        } else {  // Send a 0
            if (p.step == 1) {
                if (offset & 2) { //Odd Space
                    result.push_back(p.zerospace);
                } else { //Event Mark
                    result.push_back(p.zeromark);
                }             
            } else {
                result.push_back(p.zeromark);
                result.push_back(p.zerospace);
            }
        } 
    }
    }

    return result.size() - offset;
};

int rfir::module::ttl::EncoderV2::encodeBytes(Params p, std::list<uint8_t>& bytes, std::list<uint16_t>& result){
    int offset = result.size();

    if (p.headermark) result.push_back(p.headermark);
    if (p.headerspace) result.push_back(p.headerspace);
    for (auto it = bytes.begin(); it != bytes.end(); it++)
    {
        encodeBits(p, *it, 8, result);
    }
    
    if (p.footermark) result.push_back(p.footermark);
    if (p.footerspace) result.push_back(p.footerspace);  

    if(!p.footermark && !p.footerspace && p.lastspace)
        result.push_back(p.lastspace);

    return result.size() - offset;   

};


int  rfir::module::ttl::EncoderV2::parseBits(const char* data, int nbits, uint64_t& result){
    for (size_t i = 0; i < nbits; i++)
    {
        data[i] == '0' ? result <<= 1 : result = (result << 1) | 1;    
    }

    return  nbits;   
};

int  rfir::module::ttl::EncoderV2::parseBytes(const char* data, int nbits, std::list<uint8_t>& result){
    int offset = 0;
    for (size_t i = 0; i < nbits / 8; i++)
    {
      uint64_t byte = 0;
      offset += parseBits(data + offset, 8, byte);
      result.push_back((uint8_t)byte); 
    }

    if (nbits % 8) {
        uint64_t byte = 0;
        offset += parseBits(data + offset, nbits % 8, byte);
        result.push_back((uint8_t)byte); 
    }

    return offset;
};



int  rfir::module::ttl::EncoderV2::parseString(const char* cdata, std::list<uint8_t>& result){
    String data(cdata);
    int idx = data.indexOf(",");

    if (idx > 0) {
        data = data.substring(0, idx);
    }

    int nbits = 0;
    data.replace(" ", "");

    int count = data.length();
    if (count > 0) {
        if (count > 2 && data[0] == '0' && data[1] == 'x') {
            for (size_t i = 2; i < count; i += 2)
            {
                char* str;
                String p = data.substring(i, i + 2);
                int len = 4 * p.length();   
            
                uint8_t byte = (uint8_t)strtol(p.c_str(), &str, 16);  
                result.push_back(byte);
                nbits += len;
            }
        } else {
            nbits = parseBytes(data.c_str(), count, result);
        }
    }

    return nbits;    

};

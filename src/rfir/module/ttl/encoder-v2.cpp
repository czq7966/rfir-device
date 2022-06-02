#include "encoder-v2.h"

bool rfir::module::ttl::EncoderV2::Params::clone(DecoderV2::Params* p){
    if (p) {
        this->nbits = p->nbits;
        this->headermark = p->headermark;
        this->headerspace = p->headerspace;
        this->onemark = p->onemark;
        this->onespace = p->onespace;
        this->zeromark = p->zeromark;    
        this->zerospace = p->zerospace;   
        this->footermark = p->footermark;
        this->footerspace = p->footerspace;
 
        this->MSBfirst = p->MSBfirst;    
        this->step = p->step;   
        this->lastspace = p->lastspace;   
        return true;
    }
    return false;    
};

int rfir::module::ttl::EncoderV2::encodeBits(Params p, uint64_t bits, int nbits, bool hdrAndFtr, std::list<uint16_t>& result){
    int offset = result.size();

    if (hdrAndFtr) {
        if (p.headermark) result.push_back(p.headermark);
        if (p.headerspace) result.push_back(p.headerspace);
    }

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

    if (hdrAndFtr) {
        if (p.footermark) result.push_back(p.footermark);
        if (p.footerspace) result.push_back(p.footerspace);  

        if(!p.footermark && !p.footerspace && p.lastspace)
            result.push_back(p.lastspace);
    }

    return result.size() - offset;
};

int rfir::module::ttl::EncoderV2::encodeBytes(Params p, std::list<uint8_t>& bytes, std::list<uint16_t>& result){
    int offset = result.size();

    if (p.headermark) result.push_back(p.headermark);
    if (p.headerspace) result.push_back(p.headerspace);
    for (auto it = bytes.begin(); it != bytes.end(); it++)
    {
        encodeBits(p, *it, 8, false, result);
    }
    
    if (p.footermark) result.push_back(p.footermark);
    if (p.footerspace) result.push_back(p.footerspace);  

    if(!p.footermark && !p.footerspace && p.lastspace)
        result.push_back(p.lastspace);

    return result.size() - offset;   

};

int rfir::module::ttl::EncoderV2::encodeBytes(Params p, uint8_t* bytes, int size, std::list<uint16_t>& result){
    std::list<uint8_t> lBytes;
    for (size_t i = 0; i < size; i++)
    {
        lBytes.push_back(bytes[i]);
    }
    return encodeBytes(p, lBytes, result);
};

int rfir::module::ttl::EncoderV2::encode(std::vector<std::string>& data, std::list<uint16_t>& result){
    int offset = result.size();
    for (size_t i = 0; i < encodeParams.size(); i++)
    {
        if (i < data.size()) {
            auto code = data[i];
            auto params = encodeParams[i];
            uint64_t bits;
            std::list<uint8_t> bytes;            

            if (params.nbits % 8 == 0) {
                auto nbits = parseString(code.c_str(), bytes);
                encodeBytes(params, bytes, result);
            } else {
                auto nbits = parseBits(code.c_str(), params.nbits, bits);
                encodeBits(params, bits, params.nbits, true, result);
            }
        }
    }

    auto count = result.size() - offset;    
    if (count > 0)
        events.onEncoded.emit(&result);
    
    return count;
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

std::string rfir::module::ttl::EncoderV2::toString(std::list<uint16_t>& data){
    String str;
    if (data.size() > 0) {
        for (auto it = data.begin(); it != data.end(); it++)
        {        
            str = str + String(*it) + ",";
        }
    }

    return str.c_str();    
};
rfir::module::ttl::EncoderV2 GTTLEncoder;

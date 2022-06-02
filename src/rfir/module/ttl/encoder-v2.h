#ifndef __RFIR_MODULE_TTL_ENCODER_V2_H__
#define __RFIR_MODULE_TTL_ENCODER_V2_H__

#include "rfir/util/platform.h"
#include "rfir/util/event-emitter.h"
#include "decoder-v2.h"


namespace rfir {
    namespace module {
        namespace ttl {
            class EncoderV2 {
            public:                
                struct Params {                    
                    uint16_t nbits = 0;
                    uint16_t headermark = 0;
                    uint32_t headerspace = 0;
                    uint16_t onemark = 0;
                    uint32_t onespace = 0;
                    uint16_t zeromark = 0;
                    uint32_t zerospace = 0;
                    uint16_t footermark = 0;
                    uint32_t footerspace = 0;
                    bool     MSBfirst = true;
                    int      step = 2;
                    uint16_t lastspace = 0; 
                    bool     clone(DecoderV2::Params* p);
                };  

                struct Events
                {
                    rfir::util::Event onEncoded;
                };
                



            public:  
                std::string             name;              
                std::vector<Params>     encodeParams;
                Events                  events;


                int encodeBits(Params p, uint64_t bits, int nbits, bool hdrAndFtr, std::list<uint16_t>& result);
                int encodeBytes(Params p, std::list<uint8_t>& bytes, std::list<uint16_t>& result);
                int encodeBytes(Params p, uint8_t* bytes, int size, std::list<uint16_t>& result);
                int encode(std::vector<std::string>& data, std::list<uint16_t>& result);


                std::string toString(std::list<uint16_t>& result);

                int  parseBits(const char* data, int nbits, uint64_t& result);
                int  parseBytes(const char* data, int nbits, std::list<uint8_t>& result);
                int  parseString(const char* data, std::list<uint8_t>& result);


                static int  parseData(String data, uint8_t* result, bool MSBFirst);
                static uint16_t* parseRaw(const char* data, int size, int& count);
                
            };
        }
    }
}

extern rfir::module::ttl::EncoderV2 GTTLEncoder;

#endif //__RFIR_MODULE_TTL_ENCODER_H__



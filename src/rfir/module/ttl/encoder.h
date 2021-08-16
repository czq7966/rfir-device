//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_TTL_ENCODER_H__
#define __RFIR_MODULE_TTL_ENCODER_H__

#include "rfir/util/platform.h"
#include "decoder.h"

namespace rfir {
    namespace module {
        namespace ttl {
            class RFIR;
            class Encoder {
            public:                

                typedef struct {
                    bool success;   // Was the match successful?
                    uint64_t data;  // The data found.
                    uint16_t used;  // How many buffer positions were used.
                } match_result_t;          


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
                    std::string   toString();
                    bool          parseFromJson(neb::CJsonObject* jp);  
                    bool          clone(Decoder::Params* p);
                };  

                struct EncodeParams
                {
                    Params* params = 0;
                    int     count = 0;
                    bool    response = 0;
                    bool    parseFromJson(neb::CJsonObject* jblocks);  
                    void    free();
                    bool    clone(EncodeParams* p);
                    bool    clone(Decoder::DecodeParams* p);
                    int     create(int size);
                };
                 

                struct EncodeResult {
                    uint16_t*   result = 0;
                    int         count = 0;
                    std::string toString();
                    void        free();
                };

                typedef std::function<void(Encoder* encoder, EncodeResult* data)> OnEncoded;  
            private:                
                EncodeParams    encodeParams;
                EncodeResult    encodeResult;
                neb::CJsonObject jEncode;
            public:
                Encoder(RFIR* rfir = 0);
                ~Encoder();

                std::string     name;
                RFIR*           rfir = 0;
                OnEncoded       onEncoded = 0;
            public:
                EncodeParams* getEncodeParams();
                int     getEncodeParamsCount();
                

                void    initEncodeResult();
                void    uninitEncodeResult();
                void    clearEncodeResult();
                EncodeResult*  getEncodeResult();
                int     getEncodeResultCount();

                int encodeData(Params p, uint64_t data, int nbits, uint16_t* result);
                int encode(Params p, uint8_t* data, int nbits, uint16_t* result);
                int encode(neb::CJsonObject* blocks,  uint16_t* result);
                int encode(neb::CJsonObject* blocks);
                int encode(rfir::module::ttl::Decoder::DecodeResults* data, uint16_t* result);
                int encode(rfir::module::ttl::Decoder::DecodeResults* data);


                std::string  toString();

                static int  parseData(const char* data, int nbits, uint8_t* result, bool MSBFirst);
                static int  parseData(const char* data, int nbits, uint64_t& result, bool MSBFirst);
                static int  parseData(String data, uint8_t* result, bool MSBFirst);
                static uint16_t* parseRaw(const char* data, int size, int& count);
                static std::string packEncodedCmd(Encoder* encoder, EncodeResult* data);
                
            };
        }
    }
}


#endif //__RFIR_MODULE_TTL_ENCODER_H__



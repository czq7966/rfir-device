#ifndef __RFIR_MODULE_TTL_DECODER_V2_H__
#define __RFIR_MODULE_TTL_DECODER_V2_H__

#include "rfir/util/platform.h"
#include "rfir/util/util.h"

#ifndef TTL_DECODER_MAX_BYTES
#define TTL_DECODER_MAX_BYTES 8
#endif

namespace rfir {
    namespace module {
        namespace ttl {
            class DecoderV2 {
            public:                

                typedef struct {
                    bool success;   // Was the match successful?
                    uint64_t data;  // The data found.
                    uint16_t used;  // How many buffer positions were used.
                } match_result_t;          

                struct DecodeResult {
                    uint8_t             bytes[TTL_DECODER_MAX_BYTES];
                    uint64_t            bits = 0;
                    uint16_t            nbits = 0;
                    bool                use_bits = 0;
                    std::string         toBitString();
                    std::string         toHexString();
                };



                struct Params {
                    bool     use_bits = false;
                    uint16_t nbits = 0;
                    uint16_t headermark = 0;
                    uint32_t headerspace = 0;
                    uint16_t onemark = 0;
                    uint32_t onespace = 0;
                    uint16_t zeromark = 0;
                    uint32_t zerospace = 0;
                    uint16_t footermark = 0;
                    uint32_t footerspace = 0;
                    uint8_t  tolerance = 30;
                    int16_t  excess = 0;
                    bool     atleast = true;                              
                    bool     MSBfirst = true;
                    uint8_t  step = 2;
                    uint16_t lastspace = 0;
                };   

                struct Events {
                    rfir::util::Event onDecoded;
                };

            public:
                std::string name;
                std::vector<Params>       decodeParams;
                Events events;

                int     _decode(uint16_t* data, int size);

                int     decode(std::list<uint16_t>& data);
                int     decode(uint16_t* data, int size, int maxTimes = INT_MAX);
                int     decode();
                String  toString();
            public:
                bool     match(uint32_t measured, uint32_t desired, uint8_t tolerance, uint16_t delta = 0);
                bool     matchMark(uint32_t measured, uint32_t desired, uint8_t tolerance, int16_t excess);
                bool     matchMarkRange(const uint32_t measured, const uint32_t desired, const uint16_t range, const int16_t excess);         
                bool     matchSpace(const uint32_t measured, const uint32_t desired, const uint8_t tolerance, const int16_t excess);
                bool     matchSpaceRange(const uint32_t measured, const uint32_t desired, const uint16_t range, const int16_t excess);
            public:
                uint32_t ticksLow(const uint32_t usecs, const uint8_t tolerance, const uint16_t delta);
                uint32_t ticksHigh(const uint32_t usecs, const uint8_t tolerance,const uint16_t delta);            
                bool     matchAtLeast(uint32_t measured, uint32_t desired, uint8_t tolerance, uint16_t delta);
                uint16_t compare(const uint16_t oldval, const uint16_t newval);    
                uint64_t reverseBits(uint64_t input, uint16_t nbits);
                uint16_t _matchGeneric(volatile uint16_t *data_ptr,
                                        uint64_t *result_bits_ptr,
                                        uint8_t *result_ptr,
                                        const bool use_bits,
                                        const uint16_t remaining,
                                        const uint16_t required,
                                        const uint16_t hdrmark,
                                        const uint32_t hdrspace,
                                        const uint16_t onemark,
                                        const uint32_t onespace,
                                        const uint16_t zeromark,
                                        const uint32_t zerospace,
                                        const uint16_t footermark,
                                        const uint32_t footerspace,
                                        const uint8_t tolerance,
                                        const int16_t excess,
                                        const bool atleast = false,                                        
                                        const bool MSBfirst = true,
                                        const uint8_t step = 2,
                                        const uint16_t lastspace = 0);       
                match_result_t matchData(volatile uint16_t *data_ptr, const uint16_t nbits,
                                        const uint16_t onemark, const uint32_t onespace,
                                        const uint16_t zeromark, const uint32_t zerospace,
                                        const uint8_t tolerance,
                                        const int16_t excess,
                                        const bool MSBfirst = true,
                                        const bool expectlastspace = true,
                                        const uint8_t step = 2);    
                match_result_t matchDataStep1(volatile uint16_t *data_ptr, const uint16_t nbits,
                                        const uint16_t onemark, const uint32_t onespace,
                                        const uint16_t zeromark, const uint32_t zerospace,
                                        const uint8_t tolerance,
                                        const int16_t excess,
                                        const bool MSBfirst = true,
                                        const bool expectlastspace = true,
                                        const uint8_t step = 2);
                uint16_t matchBytes(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                                    const uint16_t remaining, const uint16_t nbytes,
                                    const uint16_t onemark, const uint32_t onespace,
                                    const uint16_t zeromark, const uint32_t zerospace,
                                    const uint8_t tolerance,
                                    const int16_t excess,
                                    const bool MSBfirst = true,
                                    const bool expectlastspace = true,
                                    const uint8_t step = 2);
                uint16_t matchGeneric(volatile uint16_t *data_ptr,
                                        uint64_t *result_ptr,
                                        const uint16_t remaining, const uint16_t nbits,
                                        const uint16_t hdrmark, const uint32_t hdrspace,
                                        const uint16_t onemark, const uint32_t onespace,
                                        const uint16_t zeromark, const uint32_t zerospace,
                                        const uint16_t footermark, const uint32_t footerspace,
                                        const bool atleast,
                                        const uint8_t tolerance,
                                        const int16_t excess,
                                        const bool MSBfirst,
                                        const uint8_t step,
                                        const uint16_t lastspace);
                uint16_t matchGeneric(volatile uint16_t *data_ptr, uint8_t *result_ptr,
                                        const uint16_t remaining, const uint16_t nbits,
                                        const uint16_t hdrmark, const uint32_t hdrspace,
                                        const uint16_t onemark, const uint32_t onespace,
                                        const uint16_t zeromark, const uint32_t zerospace,
                                        const uint16_t footermark,
                                        const uint32_t footerspace,
                                        const bool atleast,
                                        const uint8_t tolerance,
                                        const int16_t excess,
                                        const bool MSBfirst,
                                        const uint8_t step,
                                        const uint16_t lastspace);
                uint16_t matchGeneric(volatile uint16_t *data_ptr, uint64_t *result_bits_ptr, uint8_t *result_ptr,
                                        const uint16_t remaining, const Params params);

            };
        }
    }
}

extern rfir::module::ttl::DecoderV2 GTTLDecoder;

#endif //__RFIR_MODULE_GPIO_TTL_SNIFFER_H__

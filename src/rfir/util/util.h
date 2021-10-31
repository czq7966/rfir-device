#ifndef __RFIR_UTIL_UTIL_H__
#define __RFIR_UTIL_UTIL_H__

#include "platform.h"
#include "config.h"

namespace rfir {
    namespace util {
        class Util {
        public:
            static uint64_t reverseBits(uint64_t input, uint16_t nbits);
            static std::string GetChipId(std::string prefix = "");
            static std::string BitsToString(uint8_t bytes[], uint16_t nbits);
            static std::string BytesToString(uint8_t bytes[], uint16_t nbytes);
        };
    }
}
                

#endif                
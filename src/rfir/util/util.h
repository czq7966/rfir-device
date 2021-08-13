#ifndef __RFIR_UTIL_UTIL_H__
#define __RFIR_UTIL_UTIL_H__

#include "platform.h"

namespace rfir {
    namespace util {
        class Util {
        public:
            static uint64_t reverseBits(uint64_t input, uint16_t nbits);
            static std::string GetChipId(std::string prefix = "");
        };
    }
}
                

#endif                
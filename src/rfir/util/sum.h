#ifndef RFIR_UTIL_SUM_H
#define RFIR_UTIL_SUM_H

#include <Arduino.h>

namespace rfir {
    namespace util {
        class Sum {
        public:
            static uint8_t Xor(uint8_t* buf, uint16_t len);
        };
    }
}
                

#endif                
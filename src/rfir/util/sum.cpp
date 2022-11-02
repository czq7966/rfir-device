#include "sum.h"

uint8_t rfir::util::Sum::Xor(uint8_t* buf, uint16_t len) {
    uint8_t result = 0;

    for (size_t i = 1; i < len; i++)
    {
       result ^= buf[i];
    }
    
    return result;
};
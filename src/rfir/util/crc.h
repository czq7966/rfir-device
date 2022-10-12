#ifndef __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_UTIL_CRC_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_UTIL_CRC_H__

#include <Arduino.h>
#include "string"

namespace rfir {
    namespace util {
        class Crc {
        public:
            static uint16_t CRC16_Tab[];
            static uint16_t Calc_Crc16(uint8_t crcbuf, uint16_t crc_16);
            static uint16_t Get_CRC16(uint8_t *pBuff, uint16_t len);
            static uint16_t Get_CRC16(uint8_t *pBuff, uint16_t len, uint8_t& crc16_low, uint8_t& crc16_high);
            static uint16_t Get_CRC16(std::string data, uint8_t& crc16_low, uint8_t& crc16_high);
            static std::string Get_CRC16_Str(std::string hexStr);
        };
    }
}
                

#endif                
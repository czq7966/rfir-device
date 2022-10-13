#ifndef __RFIR_UTIL_UTIL_H__
#define __RFIR_UTIL_UTIL_H__

#include "platform.h"
#include "config.h"

namespace rfir {
    namespace util {
        class Util {
        protected:
            static void* on_delay_reset(void* arg, void* p);
        public:
            static uint64_t reverseBits(uint64_t input, uint16_t nbits);
            static std::string GetChipId(std::string prefix = "");
            static int GetChipId(char* buf, const char* prefix = 0);
            static std::string GetMacAddress();
            static int GetMacAddress(char* buf);
            static std::string BitsToString(uint8_t bytes[], uint16_t nbits);
            static std::string BytesToString(uint8_t bytes[], uint16_t nbytes);
            static std::string BytesToHexString(uint8_t bytes[], uint16_t nbytes, bool revert = false, char separater = ' ');
            static int  BytesToHexChar(uint8_t bytes[], uint16_t nbytes, char* buf, bool revert = false, char separater = ' ');
            static int  StringToBytes(const char* data, int nbits, uint8_t* bytes);
            static int  StringToBits(const char* data, int nbits, uint64_t& result);
            static int  StringToBytes(std::string data, uint8_t* bytes);   
            static void Reset();
            static void DelayReset(int timeout_ms);
            static std::string ToLower(const char* data);
            static std::string ToUpper(const char* data);
        };
    }
}
                

#endif                
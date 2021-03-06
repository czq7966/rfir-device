#ifndef RFIR_UTIL_DEBUGER_H
#define RFIR_UTIL_DEBUGER_H

#include "Arduino.h"
#include "Print.h"

namespace rfir {
    namespace util {
        class Debuger: public Print
        {
        public:
            void begin(unsigned long baud);
#ifdef ESP8266
            void begin(unsigned long baud, SerialConfig config);
#else
            void begin(unsigned long baud, uint32_t config);
#endif

            // void print(const char* bytes);
            // void println(const char* bytes);
            // void println(String bytes);
            // void printf(const char *format, ...);
            // void flush(void);
            size_t write(uint8_t c) override;;
            size_t write(const uint8_t *buffer, size_t size)override;
            int read(void);
            int read(char* buffer, size_t size);
            int read(uint8_t* buffer, size_t size);
            size_t readBytes(char* buffer, size_t size);
            size_t readBytes(uint8_t* buffer, size_t size);
            String readStringUntil(char c);
        };
    }
}

extern rfir::util::Debuger RFIRDebuger; 

#endif
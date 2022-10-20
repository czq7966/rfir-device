#ifndef RFIR_UTIL_DEBUGER_H
#define RFIR_UTIL_DEBUGER_H

#include "Arduino.h"
#include "Print.h"

namespace rfir {
    namespace util {
        class Debuger: public Print
        {
        public:
            bool enabled = true;
        public:
            void begin(unsigned long baud);
#ifdef ESP8266
            void begin(unsigned long baud, SerialConfig config);
#else
            void begin(unsigned long baud, uint32_t config);
#endif

            size_t write(uint8_t c) override;;
            size_t write(const uint8_t *buffer, size_t size)override;
            int read(void);
            int read(char* buffer, size_t size);
            int read(uint8_t* buffer, size_t size);
            size_t readBytes(char* buffer, size_t size);
            size_t readBytes(uint8_t* buffer, size_t size);
            String readStringUntil(char c);

            size_t print(const __FlashStringHelper *);
            size_t print(const String &);
            size_t print(const char[]);
            size_t print(char);
            size_t print(unsigned char, int = DEC);
            size_t print(int, int = DEC);
            size_t print(unsigned int, int = DEC);
            size_t print(long, int = DEC);
            size_t print(unsigned long, int = DEC);
            size_t print(long long, int = DEC);
            size_t print(unsigned long long, int = DEC);
            size_t print(double, int = 2);
            size_t print(const Printable&);

            size_t println(const __FlashStringHelper *);
            size_t println(const String &s);
            size_t println(const char[]);
            size_t println(char);
            size_t println(unsigned char, int = DEC);
            size_t println(int, int = DEC);
            size_t println(unsigned int, int = DEC);
            size_t println(long, int = DEC);
            size_t println(unsigned long, int = DEC);
            size_t println(long long, int = DEC);
            size_t println(unsigned long long, int = DEC);
            size_t println(double, int = 2);
            size_t println(const Printable&);
            size_t println(void);
        };
    }
}

extern rfir::util::Debuger GDebuger; 

#endif
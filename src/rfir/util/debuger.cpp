#include "Debuger.h"
#include <stdarg.h>
#include <stdio.h>
#include "Arduino.h"




void rfir::util::Debuger::begin(unsigned long baud) {
    if (this->enabled)
        Serial.begin(baud);
};
#ifdef ESP8266
void rfir::util::Debuger::begin(unsigned long baud, SerialConfig config){
    if (this->enabled)
        Serial.begin(baud, config);    
};
#else
void rfir::util::Debuger::begin(unsigned long baud, uint32_t config){};
    if (this->enabled)
        Serial.begin(baud, config);
#endif
// void rfir::util::Debuger::print(const char* bytes){};
// void rfir::util::Debuger::println(const char* bytes){};
// void rfir::util::Debuger::println(String bytes){};
// void rfir::util::Debuger::println(int num){};
// void rfir::util::Debuger::println(uint16_t num){};
// void rfir::util::Debuger::println(bool b){};
// void rfir::util::Debuger::println(size_t b){};
// void rfir::util::Debuger::printf(const char *format, ...){};
// void rfir::util::Debuger::flush(void) {};
size_t rfir::util::Debuger::write(uint8_t c){ return 1;}
size_t rfir::util::Debuger::write(const uint8_t *buffer, size_t size){ return size;}
int rfir::util::Debuger::read(void) {  return 0; }
int rfir::util::Debuger::read(char* buffer, size_t size) { return 0;}
int rfir::util::Debuger::read(uint8_t* buffer, size_t size) { return 0;}
size_t rfir::util::Debuger::readBytes(char* buffer, size_t size) { return 0;}
size_t rfir::util::Debuger::readBytes(uint8_t* buffer, size_t size) { return 0;}
String rfir::util::Debuger::readStringUntil(char c) {return "";}


size_t rfir::util::Debuger::printf(const char * format, ...){
    size_t result = 0;
    if (this->enabled) {
        va_list args;
        va_start(args, format);
        result = Serial.printf(format, args);
        va_end(args);
    }

    return result;

};
size_t  rfir::util::Debuger::printf_P(PGM_P format, ...) {
    size_t result = 0;
    if (this->enabled) {
        va_list args;
        va_start(args, format);
        result = Serial.printf_P(format, args);
        va_end(args);
    }

    return result;
};
size_t rfir::util::Debuger::print(const __FlashStringHelper * p){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};
size_t rfir::util::Debuger::print(const String & p){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};
size_t rfir::util::Debuger::print(const char p[]){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};
size_t rfir::util::Debuger::print(char p){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};
size_t rfir::util::Debuger::print(unsigned char p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(int p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(unsigned int p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(long p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(unsigned long p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(long long p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(unsigned long long p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(double p, int h){
    if (this->enabled) 
        return Serial.print(p, h);
    return 0;
};
size_t rfir::util::Debuger::print(const Printable& p){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};

size_t rfir::util::Debuger::println(const __FlashStringHelper * p){
    if (this->enabled) 
        return Serial.print(p);
    return 0;
};
size_t rfir::util::Debuger::println(const String &p){
    if (this->enabled) 
        return Serial.println(p);
    return 0;
};
size_t rfir::util::Debuger::println(const char p[]){
    if (this->enabled) 
        return Serial.println(p);
    return 0;
};
size_t rfir::util::Debuger::println(char p){
    if (this->enabled) 
        return Serial.println(p);
    return 0;
};
size_t rfir::util::Debuger::println(unsigned char p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(int p, int h){
    return 0;
};
size_t rfir::util::Debuger::println(unsigned int p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(long p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(unsigned long p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(long long p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(unsigned long long p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(double p, int h){
    if (this->enabled) 
        return Serial.println(p, h);
    return 0;
};
size_t rfir::util::Debuger::println(const Printable& p){
    if (this->enabled) 
        return Serial.println(p);
    return 0;
};
size_t rfir::util::Debuger::println(){
    if (this->enabled) 
        return Serial.println();
    return 0;
};


rfir::util::Debuger GDebuger; 

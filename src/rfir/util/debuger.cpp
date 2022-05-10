#include "debuger.h"


void rfir::util::Debuger::begin(unsigned long baud) {};
void rfir::util::Debuger::begin(unsigned long baud, SerialConfig config){};
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

rfir::util::Debuger RFIRDebuger; 

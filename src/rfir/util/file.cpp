#include "file.h"
#include <iostream>
#include <fstream>

rfir::util::TxtFile::TxtFile(const char * fn) {
    this->fn = fn;
}

int rfir::util::TxtFile::readString(String& str) {
#ifdef ESP32
    File file = SPIFFS.open(this->fn.c_str(), FILE_READ);
#endif
#ifdef ESP8266
    File file = SPIFFS.open(this->fn.c_str(), "r");
#endif

    if (!file)
        return 0;

    while (file.available())
    {
        str += (char)file.read();
    }

    return 1;
}

int  rfir::util::TxtFile::writeString(String txt) {
    return writeString(txt.c_str(), txt.length());
}

int rfir::util::TxtFile::writeString(const char* buf, uint32_t size) {
#ifdef ESP32
    File file = SPIFFS.open(this->fn.c_str(), FILE_WRITE);
#endif
#ifdef ESP8266
    File file = SPIFFS.open(this->fn.c_str(), "w");
#endif    
     
    if (!file)
        return -1;

    int count = file.write((uint8_t *)buf, size);
    file.flush();
    file.close();

    return count;   
}
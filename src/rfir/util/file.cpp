#include "file.h"
#include <iostream>
#include <fstream>

bool rfir::util::File::remove(const char * fn) {
    if (FileFS.exists(fn)) {
        return FileFS.remove(fn);
    }
    return 1;
}

rfir::util::TxtFile::TxtFile(const char * fn) {
    this->fn = std::string(fn);
}

bool rfir::util::TxtFile::begin() {
#if ESP8266
    if (!FileFS.begin())
    {
        // FileFS.format();
#else
    if (!FileFS.begin(true))
    {
#endif        
        if (!FileFS.begin())
        {
            DEBUGER.println("LittleFS begin failed");
            return false;
        }
    }    

    return true;
}

int rfir::util::TxtFile::readString(std::string& str) {
    if (begin()) {
        fs::File file;
        if (FileFS.exists(this->fn.c_str())) {
            file = FileFS.open(this->fn.c_str(), "r");
        } else return 0;

        while (file.available())
        {
            str += (char)file.read();
        }
        file.close();

        return 1;        
    }

    return 0;
    
//     fs::File file;
//     if (FileFS.exists(this->fn.c_str()))
// #ifdef ESP32
//         file = FileFS.open(this->fn.c_str(), FILE_READ);
// #endif
// #ifdef ESP8266
//         file = FileFS.open(this->fn.c_str(), "r");
// #endif    
//     else return 0;

    
//     while (file.available())
//     {
//         str += (char)file.read();
//     }
//     file.close();

//     return 1;
}

int  rfir::util::TxtFile::writeString(std::string txt) {
    return writeString(txt.c_str(), txt.length());
}

int rfir::util::TxtFile::writeString(const char* buf, uint32_t size) {
    if (begin()) {
        fs::File file = FileFS.open(this->fn.c_str(), "w");
        if (!file)
            return -1;

        int count = file.write((uint8_t *)buf, size);
        file.flush();
        file.close();

        return count;   
    } 
    return -1;

// #ifdef ESP32
//     fs::File file = FileFS.open(this->fn.c_str(), FILE_WRITE);
// #endif
// #ifdef ESP8266
//     fs::File file = FileFS.open(this->fn.c_str(), "w");
// #endif    
     
//     if (!file)
//         return -1;

//     int count = file.write((uint8_t *)buf, size);
//     file.flush();
//     file.close();

//     return count;   
}
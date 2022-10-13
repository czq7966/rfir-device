#include "file.h"
#include <iostream>
#include <fstream>
#include "debuger.h"

bool rfir::util::File::remove(const char * fn) {
    if (FileFS.exists(fn)) {
        return FileFS.remove(fn);
    }
    return 1;
}

rfir::util::File::File(const char * fn) {
    this->fn = fn;
}

bool rfir::util::File::begin() {
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
            GDebuger.println(F("LittleFS begin failed"));
            return false;
        }
    }    

    return true;
}

int rfir::util::File::readString(std::string& str) {
    int result = 0;
    if (begin()) {
        fs::File file;
        if (FileFS.exists(this->fn)) {
            file = FileFS.open(this->fn, "r");
        } else return -1;

        while (file.available())
        {
            str += (char)file.read();
            result++;
        }
        file.close();
    }

    return result;
    
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

int  rfir::util::File::writeString(std::string txt) {
    return writeString(txt.c_str(), txt.length());
}

int rfir::util::File::writeString(const char* buf, uint32_t size) {
    if (begin()) {
        fs::File file = FileFS.open(this->fn, "w");
        if (!file)
            return -1;

        int count = file.write((uint8_t *)buf, size);
        file.flush();
        file.close();

        return count;   
    } 
    return -1; 
}

int  rfir::util::File::read(char* buf, size_t maxSize) {
    int result = 0;
    if (begin()) {
        fs::File file;
        if (FileFS.exists(this->fn)) {
            file = FileFS.open(this->fn, "r");
        } else return -1;

        while (file.available())
        {
            buf[result] = (char)file.read();
            result++;
        }
        file.close();
    }

    return result;
};

int  rfir::util::File::write(const char* buf, size_t size) {
    if (begin()) {
        fs::File file = FileFS.open(this->fn, "w");
        if (!file)
            return -1;

        int count = file.write((uint8_t *)buf, size);
        file.flush();
        file.close();

        return count;   
    } 
    return -1;
};

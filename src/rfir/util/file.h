#ifndef __RFIR_UTIL_FILE_H__
#define __RFIR_UTIL_FILE_H__

#include <stdint.h>
#include "platform.h"


// #ifndef FileFS
// #define FileFS SPIFFS
// #endif

namespace rfir {
    namespace util {
        class File {
        public:
            static bool remove(const char* path);
        };

        class TxtFile: File {   
        private:
            std::string fn;
        public:
            TxtFile(const char * fn);
            bool begin();
            int  readString(std::string& txt);
            int  writeString(std::string txt);
            int  writeString(const char* buf, uint32_t size);
        };
    }
}

#endif  // __RFIR_UTIL_FILE_H__

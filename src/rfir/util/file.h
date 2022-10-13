#ifndef __RFIR_UTIL_FILE_H__
#define __RFIR_UTIL_FILE_H__

#include <stdint.h>
#include "platform.h"

namespace rfir {
    namespace util {
        class File {
        public:
            static bool remove(const char* path);
            const char* fn;
            File(const char * fn);
            bool begin();
            int  readString(std::string& txt);
            int  writeString(std::string txt);
            int  writeString(const char* buf, uint32_t size);
            int  read(char* buf, size_t maxSize);
            int  write(const char* buf, size_t size);
        };


    }
}

#endif  // __RFIR_UTIL_FILE_H__

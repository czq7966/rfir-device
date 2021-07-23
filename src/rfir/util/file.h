#ifndef __RFIR_UTIL_FILE_H__
#define __RFIR_UTIL_FILE_H__

#include <stdint.h>
#include "platform.h"

namespace rfir {
    namespace util {
        class TxtFile {   
        private:
            String fn;
        public:
            TxtFile(const char * fn);
            int  readString(String& txt);
            int  writeString(String txt);
            int  writeString(const char* buf, uint32_t size);
        };
    }
}

#endif  // __RFIR_UTIL_FILE_H__

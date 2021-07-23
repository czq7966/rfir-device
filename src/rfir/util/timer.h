#ifndef __RFIR_UTIL_TIMER_H__
#define __RFIR_UTIL_TIMER_H__

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "platform.h"

namespace rfir {
    namespace util {
        class Timer {   
        public:
            Timer();
            void reset();
            uint32_t elapsed();
        private:
            uint32_t start;
        };
    }
}

#endif  // __RFIR_UTIL_TIMER_H__

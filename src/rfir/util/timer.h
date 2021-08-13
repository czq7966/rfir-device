#ifndef __RFIR_UTIL_TIMER_H__
#define __RFIR_UTIL_TIMER_H__


#include <stdint.h>

namespace rfir {
    namespace util {
      class Timer {
      public:
        Timer();
        void reset();
        uint32_t elapsed();
      private:
        uint32_t start = 0;  
      };

      class TimerMs {
      public:
        TimerMs();
        void reset();
        uint32_t elapsed();

      private:
        uint32_t start =0;
      };
    }
}
#endif  

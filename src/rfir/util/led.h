#ifndef __RFIR_UTIL_LED_H__
#define __RFIR_UTIL_LED_H__

#include "platform.h"
#include "jled.h"

namespace rfir {
    namespace util {
        class Led {
        public:
            JLed* led = 0;
        public:
            void start(JLed* pled);
            void loop();
        public:
            bool idle();
            void update();
            void stop();
        };
    }
}
                
extern rfir::util::Led GLed;

#endif                
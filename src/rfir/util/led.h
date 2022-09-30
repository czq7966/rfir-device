#ifndef __RFIR_UTIL_LED_H__
#define __RFIR_UTIL_LED_H__

#include "platform.h"
#include "jled.h"

namespace rfir {
    namespace util {
        class Led {
        public:
            JLed* led = 0;
            void* owner = 0;
        public:
            void start(JLed* pled);
            void start(JLed* pled, void* owner);
            void stop();
            void stop(void* owner);
            void loop();
        public:
            bool idle();
            void update();            
        };
    }
}
                
extern rfir::util::Led GLed;

#endif                
#ifndef __RFIR_UTIL_INTERRUPT_H__
#define __RFIR_UTIL_INTERRUPT_H__

#include "platform.h"

namespace rfir {
    namespace util {
        class Interrupt {
        public:
            struct PinEvent {
                int pin = 0;
                int value = -1;
                bool started = false;
                void* arg;
                Event* event;                
            };
            typedef rfir::util::MapFactory<int32_t, PinEvent*> PinEvents;
        public:
            PinEvents pinEvents;
            bool m_started = false;
        public:
            void start(int pin = -1);
            void stop(int pin = -1);
            PinEvent* getPinEvent(int pin);
        public:
            static IRAM_ATTR void OnPinInterrupt(void* arg);
        };
    }
}

extern rfir::util::Interrupt GInterrupt;    

#endif                
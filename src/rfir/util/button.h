#ifndef __RFIR_UTIL_BUTTON_H__
#define __RFIR_UTIL_BUTTON_H__

#include "platform.h"
#include "event-emitter.h"
#include "debuger.h"

namespace rfir {
    namespace util {
        class Button {
        public:
            struct KeyTime {
                unsigned long prePressed = 0;
                unsigned long pressed = 0;
                unsigned long released = 0;
            };

            struct Events {
                rfir::util::Event onPressed;
                rfir::util::Event onReleased;
                rfir::util::Event onLongPressed;
                rfir::util::Event onLongReleased;
                rfir::util::Event onBetweenReleased;
                rfir::util::Event onClick;
                rfir::util::Event onLongClick;                
            }; 

        public:
            int pin = -1;
            int pressedValue = HIGH;
            Events events;
            KeyTime keyTime;
        public:
            Button();
        public:
            void* onClickCheck(void* arg, void* p);
            void* onLongClickCheck(void* arg, void* p);
            void* onLongPressedCheck(void* arg, void* p);
            void* onLongReleasedCheck(void* arg, void* p);
            void* onBetweenReleasedCheck(void* arg, void* p);
        public:
            void start(int pin, int pressedValue = LOW);
            void start();
            void stop();
            void loop();
        public:
            void* onPinInterrupt(void* arg, void* p);        
        };
    }
}
                
extern rfir::util::Button GButton;

#endif                
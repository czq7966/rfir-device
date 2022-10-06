#ifndef __RFIR_UTIL_MEM_H__
#define __RFIR_UTIL_MEM_H__

#include "platform.h"
#include "event-emitter.h"

namespace rfir {
    namespace util {
        class Mem {
        public:
            struct Events
            {
                rfir::util::Event lower;               
            };
        public:
            int lowSize = MQTT_BUFFER_SIZE;            
        public:
            void start();
            void loop();
        public:
            Events events;
        public:
            void lowCheck();
         
        };
    }
}
                
extern rfir::util::Mem GMem;

#endif                
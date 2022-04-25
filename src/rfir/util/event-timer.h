#ifndef _RFIR_UTIL_EVENT_TIMER_H_
#define _RFIR_UTIL_EVENT_TIMER_H_

#include "event-emitter.h"

namespace rfir {
    namespace util {
        class EventTimer  {
        public:
            static uint32_t HandlerIndex;        
            std::map<uint32_t, unsigned long> m_handlers;
            std::vector<std::pair<uint32_t, unsigned long>> m_handlersVec;    
            EventEmitter m_eventEmitter; 
        public:
            virtual void start();  
            virtual void loop();            
        public:
            struct CompByValue {
                bool operator()(const std::pair<uint32_t, unsigned long>& lhs, const std::pair<uint32_t, unsigned long>& rhs) {
                    return  lhs.second < rhs.second;
                }
            };

            int generateHandler();
            void addHandler(uint32_t handler, unsigned long outtime, EventEmitter::Callback cb, void* cbArg = NULL);
            void removeHandler(uint32_t handler, bool sorted = true);
            void sortHandlers();
            void emitHandler(uint32_t handler);
            bool checkAndEmitTimeout();

            int delay(int timeout_ms, EventEmitter::Callback cb, void* cbArg = NULL , uint32_t handler = 0);
            void remove(uint32_t handler);
            void clear();
            int newHandler();
            int getCount();

        };        
    }
}

extern rfir::util::EventTimer GEventTimer;

#endif
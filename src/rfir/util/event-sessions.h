#ifndef _RFIR_UTIL_EVENT_SESSIONS_H_
#define _RFIR_UTIL_EVENT_TIMER_H_

#include "event-emitter.h"

namespace rfir {
    namespace util {
        class EventTimer  {
        public:
            int m_handler_index = 0;        
            std::map<int, unsigned long> m_handlers;
            std::vector<std::pair<int, unsigned long>> m_handlersVec;    
            EventEmitter m_eventEmitter; 
        public:
            virtual void start();  
            virtual void loop();            
        public:
            struct CompByValue {
                bool operator()(const std::pair<int, unsigned long>& lhs, const std::pair<int, unsigned long>& rhs) {
                    return  lhs.second < rhs.second;
                }
            };

            int generateHandler();
            void addHandler(int handler, unsigned long outtime, EventEmitter::Callback cb, void* cbArg = NULL);
            void removeHandler(int handler, bool sorted = true);
            void sortHandlers();
            void emitHandler(int handler);
            bool checkAndEmitTimeout();

            int delay(int timeout, EventEmitter::Callback cb, void* cbArg = NULL , int handler = 0);
            void remove(int handler);
            void clear();
            int newHandler();
            int getCount();

        };        
    }
}

extern rfir::util::EventTimer EventTimer;

#endif
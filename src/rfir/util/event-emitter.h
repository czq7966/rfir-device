#ifndef _RFIR_UTIL_EVENT_EMITTER_H_
#define _RFIR_UTIL_EVENT_EMITTER_H_

#include <map>
#include <string>
#include <functional>
#include "map-factory.h"

namespace rfir {
    namespace util {
        class EventEmitter
        {
        public:
            ~EventEmitter();
            void clear();
        public:
            typedef std::function<void*(void*, void*)> Callback;
            struct Event {
                Callback        callback = 0;
                void*           cbArg = 0;
                bool            once = false;   
                void*           ckArg = 0;
            };

           
            typedef rfir::util::MapFactory<int32_t, Event> Events;
            Events      m_events;
            Callback    m_cb_check = 0;
        public:
            int getEventCount();
        public:
            void on(const int32_t id, Callback cb, void* cbArg = NULL, bool once = false, void* ckArg = NULL);
            void on(const void* id, Callback cb, void* cbArg = NULL, bool once = false, void* ckArg = NULL);

            void off(const int32_t id);
            void off(const void* id);

            void once(const int32_t id, Callback cb, void* cbArg = NULL, void* ckArg = NULL);
            void once(const void* id, Callback cb, void* cbArg = NULL, void* ckArg = NULL);    

            void*  emit(void* p, bool check = false, Callback cbCheck = 0);
            void*  emit(const int32_t id, void* p);
            void*  emit(const void* id, void* p);

            void  setCheck(Callback ck);
        };    

        class Event: public  EventEmitter{
        public:
            void add(const int32_t id, EventEmitter::Callback cb, void* cbArg = NULL, void* ckArg = NULL);
            void add(const void* id, EventEmitter::Callback cb, void* cbArg = NULL, void* ckArg = NULL);

            void remove(const int32_t id);
            void remove(const void* id);        
        };
    }
}

#endif
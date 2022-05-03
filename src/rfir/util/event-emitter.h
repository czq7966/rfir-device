#ifndef _RFIR_UTIL_EVENT_EMITTER_H_
#define _RFIR_UTIL_EVENT_EMITTER_H_

#include <map>
#include <string>
#include <functional>
#include "map-factory.h"

namespace rfir {
    namespace util {
    //     class EventEmitter
    //     {
    //     public:
    //         ~EventEmitter();
    //         void clear();
    //     public:
    //         typedef std::function<void*(void*, void*)> Callback;
    //         struct Event {
    //             Callback        callback = 0;
    //             void*           cbArg = 0;
    //             bool            once = false;
    //         };

    //         // typedef std::map<std::string, Event*> Events;
    //         // typedef std::map<std::string, Events*> Listeners;
            
    //         typedef rfir::util::MapFactory<std::string, Event*> Events;
    //         typedef rfir::util::MapFactory<std::string, Events*> Listeners;

    //         Listeners m_listeners;
    //     public:
    //         enum EEmit2Result {
    //             None =                      0b0,
    //             PreventDefault =            0b1,
    //             PreventRoot =               0b10,
    //         } ;        

    //         int getEventCount();
    //         int getEventCount(const std::string name);
    //     public:
    //         void on(const std::string name, const std::string id, Callback cb, void* cbArg = NULL, bool once = false);
    //         void on(const std::string name, const int32_t id, Callback cb, void* cbArg = NULL, bool once = false);
    //         void on(const std::string name, const void* id, Callback cb, void* cbArg = NULL, bool once = false);

    //         void off(const std::string name, const std::string id);
    //         void off(const std::string name, const int32_t id);
    //         void off(const std::string name, const void* id);

    //         void once(const std::string name, const std::string id, Callback cb, void* cbArg = NULL);
    //         void once(const std::string name, const int32_t id, Callback cb, void* cbArg = NULL);
    //         void once(const std::string name, const void* id, Callback cb, void* cbArg = NULL);    

    //         void*  emit(std::string name, void* p);
    //         void*  emit2(std::string name, void* p);
    //     };

    //    class Event{
    //         EventEmitter m_eventEmitter;
    //         std::string m_name;
    //     public:
    //         Event();
    //         Event(std::string name);

    //         int getCount();

    //         void add(const std::string id, EventEmitter::Callback cb, void* cbArg = NULL);
    //         void add(const int32_t id, EventEmitter::Callback cb, void* cbArg = NULL);
    //         void add(const void* id, EventEmitter::Callback cb, void* cbArg = NULL);

    //         void remove(const std::string id);
    //         void remove(const int32_t id);
    //         void remove(const void* id);

    //         void once(const std::string id, EventEmitter::Callback cb, void* cbArg = NULL);
    //         void once(const int32_t id, EventEmitter::Callback cb, void* cbArg = NULL);
    //         void once(const void* id, EventEmitter::Callback cb, void* cbArg = NULL);    

    //         void*  emit(void* p);
    //         void*  emit2(void* p);

    //     };      


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
            };

           
            typedef rfir::util::MapFactory<int32_t, Event> Events;
            Events m_events;
        public:
            int getEventCount();
        public:
            void on(const int32_t id, Callback cb, void* cbArg = NULL, bool once = false);
            void on(const void* id, Callback cb, void* cbArg = NULL, bool once = false);

            void off(const int32_t id);
            void off(const void* id);

            void once(const int32_t id, Callback cb, void* cbArg = NULL);
            void once(const void* id, Callback cb, void* cbArg = NULL);    

            void*  emit(void* p);
            void*  emit(const int32_t id, void* p);
            void*  emit(const void* id, void* p);
        };    

        class Event: public  EventEmitter{
        public:
            void add(const int32_t id, EventEmitter::Callback cb, void* cbArg = NULL);
            void add(const void* id, EventEmitter::Callback cb, void* cbArg = NULL);

            void remove(const int32_t id);
            void remove(const void* id);        
        };
    }
}

#endif
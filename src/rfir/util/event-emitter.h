#ifndef _RFIR_UTIL_EVENT_EMITTER_H_
#define _RFIR_UTIL_EVENT_EMITTER_H_

#include <map>
#include <string>
#include <functional>

namespace rfir {
    namespace util {
        class EventEmitter
{
public:
    typedef std::function<void*(void*, void*)> Callback;
    struct Event {
        Callback        callback = 0;
        void*           cbArg;
        bool            once = false;
    };

    typedef std::map<std::string, Event*> Events;
    typedef std::map<std::string, Events*> Listeners;

    Listeners m_listeners;


public:
    void on(const std::string name, const std::string id, Callback cb, void* cbArg = NULL, bool once = false);
    void on(const std::string name, const int64_t id, Callback cb, void* cbArg = NULL);
    void on(const std::string name, const void* id, Callback cb, void* cbArg = NULL);

    void off(const std::string name, const std::string id);
    void off(const std::string name, const int64_t id);
    void off(const std::string name, const void* id);

    void once(const std::string name, const std::string id, Callback cb, void* cbArg = NULL);
    void once(const std::string name, const int64_t id, Callback cb, void* cbArg = NULL);
    void once(const std::string name, const void* id, Callback cb, void* cbArg = NULL);    

    void*  emit(std::string name, void* p);
};
    }
}

#endif
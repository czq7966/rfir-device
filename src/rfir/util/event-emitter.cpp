#include "event-emitter.h"



void rfir::util::EventEmitter::on(const std::string name, const std::string id, Callback cb, void* cbArg, bool once) {
    off(name, id);

    auto event = new Event();
    event->callback = cb;
    event->once = once;
    event->cbArg = cbArg;

    auto lit = m_listeners.find(name);
    if (lit == m_listeners.end()) {
        m_listeners[name] = new Events();
    }

    auto events = m_listeners[name];
    (*events)[id] = event;
};

void rfir::util::EventEmitter::on(const std::string name, const int64_t id, Callback cb, void* cbArg) {
    on(name, std::to_string(id), cb, cbArg);
};

void rfir::util::EventEmitter::on(const std::string name, const void* id, Callback cb, void* cbArg) {
    on(name, int64_t(id) , cb, cbArg);
};

void rfir::util::EventEmitter::off(const std::string name, const std::string id) {
    auto lit = m_listeners.find(name);
    if (lit != m_listeners.end()) {
        auto events = lit->second;  
        auto eit = events->find(id);
        if (eit != events->end()) {
            auto event = eit->second;
            events->erase(eit);
            delete event;
        }
        if (events->size() == 0) {            
            m_listeners.erase(lit);
            delete events;
        }
    }
};

void rfir::util::EventEmitter::off(const std::string name, const int64_t id) {
     off(name, std::to_string(id));

};
void rfir::util::EventEmitter::off(const std::string name, const void* id) {
    off(name, int64_t(id));
};   


void rfir::util::EventEmitter::once(const std::string name, const std::string id, Callback cb, void* cbArg) {
    on(name, id, cb, cbArg, true);
};
void rfir::util::EventEmitter::once(const std::string name, const int64_t id, Callback cb, void* cbArg) {
    once(name, std::to_string(id), cb);
};
void rfir::util::EventEmitter::once(const std::string name, const void* id, Callback cb, void* cbArg){
    once(name, int64_t(id) , cb, cbArg);
};  

void* rfir::util::EventEmitter::emit(std::string name, void* p) {
    auto lit = m_listeners.find(name);
    if (lit != m_listeners.end()) {
        auto events = lit->second;  
        auto it = events->begin();
        while (it != events->end()) {
            auto id = it->first;
            auto event = it->second;            
            it++;
            if (event->callback) {
                event->callback(event->cbArg, p);
            }

            if (event->once) {
                off(name, id);
            }
        }        
    }   

    return 0;
}
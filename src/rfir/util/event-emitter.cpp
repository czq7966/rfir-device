#include "event-emitter.h"


rfir::util::EventEmitter::~EventEmitter(){
    clear();
};
void rfir::util::EventEmitter::clear(){
    while (true)
    {
        auto it = m_listeners.begin();
        if (it != m_listeners.end()) {
            off(it->first, it->second->begin()->first);
            continue;
        } 

        break;
    }
    
};

int rfir::util::EventEmitter::getEventCount() {
    int count = 0;
    for (auto  it = m_listeners.begin(); it != m_listeners.end() ; it++) {
        count = count + it->second->size();
    }
    return count;
}

int rfir::util::EventEmitter::getEventCount(const std::string name) {
    auto it = m_listeners.find(name);
    if (it != m_listeners.end()) {
        return it->second->size();
    }
    return 0;
}

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

void rfir::util::EventEmitter::on(const std::string name, const int64_t id, Callback cb, void* cbArg, bool once) {
    on(name, std::to_string(id), cb, cbArg, once);
};

void rfir::util::EventEmitter::on(const std::string name, const void* id, Callback cb, void* cbArg, bool once) {
    on(name, int64_t(id) , cb, cbArg, once);
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
    on(name, id, cb, cbArg, true);
};
void rfir::util::EventEmitter::once(const std::string name, const void* id, Callback cb, void* cbArg){
    on(name, id, cb, cbArg, true);
};  

void* rfir::util::EventEmitter::emit(std::string name, void* p) {
    auto lit = m_listeners.find(name);
    void* result = 0;
    if (lit != m_listeners.end()) {
        auto events = lit->second;  
        auto it = events->begin();        
        while (it != events->end()) {
            auto id = it->first;
            auto event = it->second;            
            it++;
            if (event->callback) {
                result = event->callback(event->cbArg, p);
            }

            if (event->once) {
                off(name, id);
            }
        }        
    }   

    return result;
}

void*  rfir::util::EventEmitter::emit2(std::string name, void* p) {
    auto lit = m_listeners.find(name);
    int preventDefault = EEmit2Result::None;    
    void* result = 0;
    if (lit != m_listeners.end()) {
        auto events = lit->second;  
        auto it = events->begin();        
        while (it != events->end()) {
            auto id = it->first;
            auto event = it->second;            
            it++;
            if (event->callback) {
                result = event->callback(event->cbArg, p);
            }

            if (event->once) {
                off(name, id);
            }

            preventDefault = preventDefault | ((int)result & EEmit2Result::PreventDefault);
            if (preventDefault) break;            
        }        
    }   

    return result;   
}


//Event

rfir::util::Event::Event() {
    this->m_name = "";
};

rfir::util::Event::Event(std::string name) {
    this->m_name = name;
};

int rfir::util::Event::getCount() {
    return this->m_eventEmitter.getEventCount(this->m_name);
};

void  rfir::util::Event::add(const std::string id, rfir::util::EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.on(this->m_name, id, cb, cbArg);
};
void  rfir::util::Event::add(const int64_t id, rfir::util::EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.on(this->m_name, id, cb, cbArg);    
};
void  rfir::util::Event::add(const void* id, rfir::util::EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.on(this->m_name, id, cb, cbArg);    
};

void  rfir::util::Event::remove(const std::string id){
    this->m_eventEmitter.off(this->m_name, id);    
};
void  rfir::util::Event::remove(const int64_t id){
    this->m_eventEmitter.off(this->m_name, id);    
};
void  rfir::util::Event::remove(const void* id){
    this->m_eventEmitter.off(this->m_name, id);    
};

void  rfir::util::Event::once(const std::string id, EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
};
void  rfir::util::Event::once(const int64_t id, EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
};
void  rfir::util::Event::once(const void* id, EventEmitter::Callback cb, void* cbArg){
    this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
};

void*   rfir::util::Event::emit(void* p){
    return this->m_eventEmitter.emit(this->m_name, p);    
};
void*   rfir::util::Event::emit2(void* p){
    return this->m_eventEmitter.emit2(this->m_name, p);
};
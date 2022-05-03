#include "event-emitter.h"
#include "WString.h"


// rfir::util::EventEmitter::~EventEmitter(){
//     clear();
// };
// void rfir::util::EventEmitter::clear(){
//     // while (true)
//     // {
//     //     auto it = m_listeners.begin();
//     //     if (it != m_listeners.end()) {
//     //         off(it->first, it->second->begin()->first);
//     //         continue;
//     //     } 

//     //     break;
//     // }
    
// };

// int rfir::util::EventEmitter::getEventCount() {
//     int count = 0;
//     auto listeners = m_listeners.getMap();

//     for (auto  it = listeners->begin(); it != listeners->end() ; it++) {
//         count = count + getEventCount(it->first);
//     }
//     return count;

//     // int count = 0;
//     // for (auto  it = m_listeners.begin(); it != m_listeners.end() ; it++) {
//     //     count = count + it->second->size();
//     // }
//     // return count;
// }

// int rfir::util::EventEmitter::getEventCount(const std::string name) {
//     Events* events = 0;
//     if (m_listeners.get(name, events))
//         return events->getSize();
//     return 0;

//     // auto it = m_listeners.find(name);
//     // if (it != m_listeners.end()) {
//     //     return it->second->size();
//     // }
//     // return 0;
// }

// void rfir::util::EventEmitter::on(const std::string name, const std::string id, Callback cb, void* cbArg, bool once) {
//     off(name, id);

//     auto event = new Event();
//     event->callback = cb;
//     event->once = once;
//     event->cbArg = cbArg;

//     Events* events;

//     if (!m_listeners.get(name, events)) {
//         events = new Events();
//         m_listeners.add(name, events);
//     }

//     events->add(id, event);


//     // off(name, id);

//     // auto event = new Event();
//     // event->callback = cb;
//     // event->once = once;
//     // event->cbArg = cbArg;

//     // auto lit = m_listeners.find(name);
//     // if (lit == m_listeners.end()) {
//     //     m_listeners[name] = new Events();
//     // }

//     // auto events = m_listeners[name];
//     // (*events)[id] = event;
// };

// void rfir::util::EventEmitter::on(const std::string name, const int32_t id, Callback cb, void* cbArg, bool once) {
//     on(name, std::string(String(id).c_str()), cb, cbArg, once);
// };

// void rfir::util::EventEmitter::on(const std::string name, const void* id, Callback cb, void* cbArg, bool once) {
//     on(name, int32_t(id) , cb, cbArg, once);
// };

// void rfir::util::EventEmitter::off(const std::string name, const std::string id) {
//     Events* events = 0;
//     if (m_listeners.get(name, events)){
//         Event* event = 0;
//         if (events->remove(id, event))
//             delete event;
//         if (events->getSize() == 0) {
//             m_listeners.remove(name);
//             delete events;
//         }
//     };


//     // auto lit = m_listeners.find(name);
//     // if (lit != m_listeners.end()) {
//     //     auto events = lit->second;  
//     //     auto eit = events->find(id);
//     //     if (eit != events->end()) {
//     //         auto event = eit->second;
//     //         events->erase(eit);
//     //         delete event;
//     //     }
//     //     if (events->size() == 0) {            
//     //         m_listeners.erase(lit);
//     //         delete events;
//     //     }
//     // }
// };

// void rfir::util::EventEmitter::off(const std::string name, const int32_t id) {
//      off(name, std::string(String(id).c_str()));
// };
// void rfir::util::EventEmitter::off(const std::string name, const void* id) {
//     off(name, int32_t(id));
// };   


// void rfir::util::EventEmitter::once(const std::string name, const std::string id, Callback cb, void* cbArg) {
//     on(name, id, cb, cbArg, true);
// };
// void rfir::util::EventEmitter::once(const std::string name, const int32_t id, Callback cb, void* cbArg) {
//     on(name, id, cb, cbArg, true);
// };
// void rfir::util::EventEmitter::once(const std::string name, const void* id, Callback cb, void* cbArg){
//     on(name, id, cb, cbArg, true);
// };  

// void* rfir::util::EventEmitter::emit(std::string name, void* p) {
//     void* result = 0;
//     Events* events; 
//     if (m_listeners.get(name, events)) {
//         auto eventsMap = events->getMap();
//         auto it = eventsMap->begin();        
//         while (it != eventsMap->end()) {
//             auto id = it->first;
//             auto event = it->second;            
//             it++;
//             if (event->callback) {
//                 result = event->callback(event->cbArg, p);
//             }

//             if (event->once) {
//                 off(name, id);
//             }
//         }  
//     }

//     return result;

//     // void* result = 0;
//     // auto lit = m_listeners.find(name);
//     // if (lit != m_listeners.end()) {
//     //     auto events = lit->second;  
//     //     auto it = events->begin();        
//     //     while (it != events->end()) {
//     //         auto id = it->first;
//     //         auto event = it->second;            
//     //         it++;
//     //         if (event->callback) {
//     //             result = event->callback(event->cbArg, p);
//     //         }

//     //         if (event->once) {
//     //             off(name, id);
//     //         }
//     //     }        
//     // }   

//     // return result;
// }

// void*  rfir::util::EventEmitter::emit2(std::string name, void* p) {
//     int preventDefault = EEmit2Result::None;    
//     void* result = 0;
//     // auto lit = m_listeners.find(name);
//     // if (lit != m_listeners.end()) {
//     //     auto events = lit->second;  
//     //     auto it = events->begin();        
//     //     while (it != events->end()) {
//     //         auto id = it->first;
//     //         auto event = it->second;            
//     //         it++;
//     //         if (event->callback) {
//     //             result = event->callback(event->cbArg, p);
//     //         }

//     //         if (event->once) {
//     //             off(name, id);
//     //         }

//     //         preventDefault = preventDefault | ((int)result & EEmit2Result::PreventDefault);
//     //         if (preventDefault) break;            
//     //     }        
//     // }   

//     return result;   
// }


// //Event

// rfir::util::Event::Event() {
//     this->m_name = "";
// };

// rfir::util::Event::Event(std::string name) {
//     this->m_name = name;
// };

// int rfir::util::Event::getCount() {
//     return this->m_eventEmitter.getEventCount(this->m_name);
// };

// void  rfir::util::Event::add(const std::string id, rfir::util::EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.on(this->m_name, id, cb, cbArg);
// };
// void  rfir::util::Event::add(const int32_t id, rfir::util::EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.on(this->m_name, id, cb, cbArg);    
// };
// void  rfir::util::Event::add(const void* id, rfir::util::EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.on(this->m_name, id, cb, cbArg);    
// };

// void  rfir::util::Event::remove(const std::string id){
//     this->m_eventEmitter.off(this->m_name, id);    
// };
// void  rfir::util::Event::remove(const int32_t id){
//     this->m_eventEmitter.off(this->m_name, id);    
// };
// void  rfir::util::Event::remove(const void* id){
//     this->m_eventEmitter.off(this->m_name, id);    
// };

// void  rfir::util::Event::once(const std::string id, EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
// };
// void  rfir::util::Event::once(const int32_t id, EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
// };
// void  rfir::util::Event::once(const void* id, EventEmitter::Callback cb, void* cbArg){
//     this->m_eventEmitter.once(this->m_name, id, cb, cbArg);
// };

// void*   rfir::util::Event::emit(void* p){
//     return this->m_eventEmitter.emit(this->m_name, p);    
// };
// void*   rfir::util::Event::emit2(void* p){
//     return this->m_eventEmitter.emit2(this->m_name, p);
// };


//EventEmitter2

rfir::util::EventEmitter::~EventEmitter(){
    clear();
};
void rfir::util::EventEmitter::clear(){
    m_events.clear();    
};

int rfir::util::EventEmitter::getEventCount() {
    return m_events.getSize();
}

void rfir::util::EventEmitter::on(const int32_t id, Callback cb, void* cbArg, bool once) {
    off(id);

    Event event;
    event.callback = cb;
    event.once = once;
    event.cbArg = cbArg;

    m_events.add(id, event);
};

void rfir::util::EventEmitter::on(const void* id, Callback cb, void* cbArg, bool once) {
    on(int32_t(id) , cb, cbArg, once);
};

void rfir::util::EventEmitter::off(const int32_t id) {
    m_events.remove(id);
};
void rfir::util::EventEmitter::off(const void* id) {
    off(int32_t(id));
};   



void rfir::util::EventEmitter::once(const int32_t id, Callback cb, void* cbArg) {
    on(id, cb, cbArg, true);
};
void rfir::util::EventEmitter::once(const void* id, Callback cb, void* cbArg){
    on(id, cb, cbArg, true);
};  

void* rfir::util::EventEmitter::emit(void* p) {
    void* result = 0;
    std::list<int32_t> keys;
    m_events.getKeys(keys);

    while (keys.size() > 0)
    {
        auto key = keys.front();
        keys.pop_front();
        result = emit(key, p);
    }
    
    return result;

}

void*  rfir::util::EventEmitter::emit(const int32_t id, void* p){
    void* result = 0;
    Event event;
    if (m_events.get(id, event)) {
        if (event.callback) {
            result = event.callback(event.cbArg, p);
        }

        if (event.once) {
            off(id);
        }    
    }
    return result;
};

void*  rfir::util::EventEmitter::emit(const void* id, void* p){
    return emit(int32_t(id), p);
};


void rfir::util::Event::add(const int32_t id, EventEmitter::Callback cb, void* cbArg){
    on(id, cb, cbArg);
};
void rfir::util::Event::add(const void* id, EventEmitter::Callback cb, void* cbArg){
     on(id, cb, cbArg);
};

void rfir::util::Event::remove(const int32_t id){
    off(id);
};
    
void rfir::util::Event::remove(const void* id){
    off(id);
};
     
#include "event-emitter.h"
#include "WString.h"


rfir::util::EventEmitter::~EventEmitter(){
    clear();
};
void rfir::util::EventEmitter::clear(){
    m_events.clear();    
};

int rfir::util::EventEmitter::getEventCount() {
    return m_events.getSize();
}

void rfir::util::EventEmitter::on(const int32_t id, Callback cb, void* cbArg, bool once, void* ckArg) {
    off(id);

    Event event;
    event.callback = cb;
    event.once = once;
    event.cbArg = cbArg;
    event.ckArg = ckArg;

    m_events.add(id, event);
};

void rfir::util::EventEmitter::on(const void* id, Callback cb, void* cbArg, bool once, void* ckArg) {
    on(int32_t(id) , cb, cbArg, once, ckArg);
};

void rfir::util::EventEmitter::off(const int32_t id) {
    m_events.remove(id);
};
void rfir::util::EventEmitter::off(const void* id) {
    off(int32_t(id));
};   



void rfir::util::EventEmitter::once(const int32_t id, Callback cb, void* cbArg, void* ckArg) {
    on(id, cb, cbArg, true, ckArg);
};
void rfir::util::EventEmitter::once(const void* id, Callback cb, void* cbArg, void* ckArg){
    on(id, cb, cbArg, true, ckArg);
};  

void*  rfir::util::EventEmitter::emit(void* p, bool check, Callback cbCheck) {
    void* result = 0;
    std::list<int32_t> keys;
    m_events.getKeys(keys);

    while (keys.size() > 0)
    {
        auto key = keys.front();
        keys.pop_front();
        if (check) {
            Event event;
            if (m_events.get(key, event)) {
                Callback check = cbCheck ? cbCheck : this->m_cb_check;
                if (check) {
                    auto ckResult = check(event.ckArg, p);
                    if (ckResult)
                        result = emit(key, p);
                }
            }
        }
        else
            result = emit(key, p);
    }
    
    return result;   
};

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

void  rfir::util::EventEmitter::setCheck(Callback ck) {
    this->m_cb_check = ck;
};

void rfir::util::Event::add(const int32_t id, EventEmitter::Callback cb, void* cbArg, void* ckArg){
    on(id, cb, cbArg, ckArg);
};
void rfir::util::Event::add(const void* id, EventEmitter::Callback cb, void* cbArg, void* ckArg){
     on(id, cb, cbArg, ckArg);
};

void rfir::util::Event::remove(const int32_t id){
    off(id);
};
    
void rfir::util::Event::remove(const void* id){
    off(id);
};
     
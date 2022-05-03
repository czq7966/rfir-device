#include "Arduino.h"
#include "event-timer.h"

uint32_t rfir::util::EventTimer::HandlerIndex = 0; 

void rfir::util::EventTimer::start(){};  
void rfir::util::EventTimer::loop(){
    checkAndEmitTimeout();
};


void rfir::util::EventTimer::sortHandlers() {
    m_handlersVec.clear();
    m_handlersVec.insert(m_handlersVec.end(), m_handlers.begin(), m_handlers.end());
    std::sort(m_handlersVec.begin(), m_handlersVec.end(), CompByValue());
};

int  rfir::util::EventTimer::generateHandler(){
    HandlerIndex++;
    return  HandlerIndex;    
};
void rfir::util::EventTimer::addHandler(uint32_t handler, unsigned long outtime, EventEmitter::Callback cb, void* cbArg){
    m_eventEmitter.once(std::string(String(handler).c_str()), (void*)this, cb, cbArg);    
    m_handlers[handler] = outtime;
    sortHandlers();
};
void rfir::util::EventTimer::removeHandler(uint32_t handler, bool sorted){
    m_eventEmitter.off(std::string(String(handler).c_str()), (void*)this);
    m_handlers.erase(handler);
    if (sorted)  {
        sortHandlers();
    }    
};

void rfir::util::EventTimer::emitHandler(uint32_t handler){
    m_handlers.erase(handler);
    sortHandlers();
    m_eventEmitter.emit(std::string(String(handler).c_str()), (void*)handler);
};

bool rfir::util::EventTimer::checkAndEmitTimeout(){
    int count = 0;
    while (true) {
        auto it = m_handlersVec.begin();
        if (it != m_handlersVec.end()) {
            if (millis() > it->second) {
                emitHandler(it->first);
                count++;
                continue;
            }
        }

        break;
    }
    return count > 0;
};

int  rfir::util::EventTimer::delay(int timeout_ms, EventEmitter::Callback cb, void* cbArg , uint32_t handler){
    Serial.print("rfir::util::EventTimer::delay:");
    Serial.println(timeout_ms);
    unsigned long outtime = millis() + timeout_ms;    
    handler = handler != 0 ? handler : generateHandler();
    addHandler(handler, outtime, cb, cbArg);    
    return handler;
};
void rfir::util::EventTimer::remove(uint32_t handler){
    removeHandler(handler);
};
void rfir::util::EventTimer::clear(){
    m_handlers.clear();
    m_handlersVec.clear();
};
int  rfir::util::EventTimer::newHandler(){
    return generateHandler();
};
int  rfir::util::EventTimer::getCount(){
    return m_handlersVec.size();
};

rfir::util::EventTimer GEventTimer;
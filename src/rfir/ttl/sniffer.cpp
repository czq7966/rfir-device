#include "sniffer.h"
#include "rfir/util/interrupt.h"


int rfir::ttl::Sniffer::Data::toBuffer(uint16_t* buffer){
    int count = 0;
    if (deltas.size() > 0) {
        for (auto it = deltas.begin(); it != deltas.end(); it++)
        {        
            buffer[count] = *it;
            count++;
        }
    }

    return count;
};

void rfir::ttl::Sniffer::Data::dump(){
    GDebuger.println("");
    GDebuger.print("[");
    for (auto it = deltas.begin(); it != deltas.end(); it++)
    {        
        GDebuger.print(*it);
        GDebuger.print(",");
    }   
    GDebuger.println("]");
};


void rfir::ttl::Sniffer::start(bool sniff){
    if (this->params.pin >= 0) {
        GInterrupt.getPinEvent(params.pin)->event->add(this, std::bind(&Sniffer::onPinInterrupt, this, std::placeholders::_1, std::placeholders::_2));
        if (sniff) this->startSniff();
    }
};

void rfir::ttl::Sniffer::stop(){
    this->stopSniff();
    if (this->params.pin >= 0)
        GInterrupt.getPinEvent(params.pin)->event->remove(this);
};

void rfir::ttl::Sniffer::loop(){
    if (this->params.enabled)
        sniffDelta();
};

void rfir::ttl::Sniffer::startSniff() {
    if (params.enabled && params.pin >= 0) {
        stopSniff();
        GInterrupt.start(params.pin);
        m_started = true;
        GDebuger.print(F("start sniff......pin = "));
        GDebuger.println(this->params.pin);
        GDebuger.println(F(" "));
    } else {
        GDebuger.print(F("start sniff failed, pin = "));
        GDebuger.println(this->params.pin);
        GDebuger.println(F(" "));
    }
};

void rfir::ttl::Sniffer::stopSniff() {
    if (params.pin >= 0) {
        GInterrupt.stop(params.pin);
        m_started = false;
        GDebuger.print(F("stop sniff......pin = "));
        GDebuger.println(this->params.pin);
        GDebuger.println(F(" "));
    }
};

bool rfir::ttl::Sniffer::sniffStarted(){
    return m_started;
};

void rfir::ttl::Sniffer::pushTTL(bool ttl){
    TimeTTL timettl;
    timettl.ttl = ttl;
    gettimeofday(&(timettl.time), NULL);     
    data.ttls.push_back(timettl);
};

void rfir::ttl::Sniffer::sniffDelta(){
    if (sniffStarted()) {
        if (data.ttls.size() > 0) {
            auto ttl =  data.ttls.front();
            data.ttls.pop_front();
            if (!timerisset(&data.first_ttl.time)) {
                data.first_ttl = ttl;
                data.last_ttl = ttl;
            } else {
                _sniffDelta(ttl);
            }
        } else if (data.deltas.size() > 0) {
            timeval delta;
            TimeTTL ttl = data.last_ttl;
            gettimeofday(&ttl.time, NULL);
            timersub(&ttl.time, &data.last_ttl.time, &delta);            
            if (delta.tv_usec >= params.maxDelta) {
                _sniffDelta(ttl);
                timerclear(&data.first_ttl.time);
                data.deltas.clear();                
            }            
        }
    }
};

void rfir::ttl::Sniffer::_sniffDelta(TimeTTL ttl){
    timeval delta;
    timersub(&ttl.time, &data.last_ttl.time, &delta);
    if (delta.tv_sec > 0 || delta.tv_usec >= UINT16_MAX) {
        delta.tv_sec = 0;
        delta.tv_usec = UINT16_MAX;
    }

    data.last_ttl = ttl;
    data.deltas.push_back(delta.tv_usec);
    if (delta.tv_usec < params.minDelta || delta.tv_usec >= params.maxDelta || data.deltas.size() >= params.maxCount) {  
        if (data.deltas.size() < params.minCount) {
            data.first_ttl = data.last_ttl;
            data.deltas.clear();
            return;
        } 
        events.onSniffed.emit(&data);
        data.first_ttl = data.last_ttl;
        data.deltas.clear();
    }
};


void* rfir::ttl::Sniffer::onPinInterrupt(void* arg, void* p) {
    auto pinEvent = (rfir::util::Interrupt::PinEvent*)p;
    this->pushTTL(pinEvent->value);
    return 0;
};


rfir::ttl::Sniffer GTTLSniffer;

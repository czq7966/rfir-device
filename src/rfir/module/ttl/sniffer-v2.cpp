#include "sniffer-v2.h"
#include "rfir/util/interrupt.h"


std::string rfir::module::ttl::SnifferV2::Data::toString(){
    String str;
    if (deltas.size() > 0) {
        str = String(first_ttl.ttl) + ",";
        for (auto it = deltas.begin(); it != deltas.end(); it++)
        {        
            str = str + String(*it) + ",";
        }
    }

    return str.c_str();
};

std::string rfir::module::ttl::SnifferV2::Params::toString(){
    char result[128] = {0};
    sprintf(result, "pin: %d, mode: %d, inverted: %d, minCount: %d, maxCount: %d, minDelta: %d, maxDelta: %d, bufSize: %d",
            pin, mode, inverted, minCount, maxCount, minDelta, maxDelta);
    return result;
};

void rfir::module::ttl::SnifferV2::start(){
    GInterrupt.getPinEvent(params.pin)->event->add(this, std::bind(&SnifferV2::onPinInterrupt, this, std::placeholders::_1, std::placeholders::_2));
};
void rfir::module::ttl::SnifferV2::loop(){
    sniffDelta();
};

void rfir::module::ttl::SnifferV2::startSniff() {
    if (params.pin > 0) {
        stopSniff();
        GInterrupt.start(params.pin);
        m_started = true;
        DEBUGER.println(("start sniff......: (" + this->name + ")" + params.toString()).c_str());
    } else {
        DEBUGER.println(("start sniff failed, pin is 0: (" + this->name + ")" +  params.toString()).c_str());
    }
};

void rfir::module::ttl::SnifferV2::stopSniff() {
    if (params.pin > 0) {
        GInterrupt.stop(params.pin);
        m_started = false;
        DEBUGER.println(("stop sniff......: (" + this->name + ")" + params.toString()).c_str());
    }
};

bool rfir::module::ttl::SnifferV2::sniffStarted(){
    return m_started;
};

void rfir::module::ttl::SnifferV2::pushTTL(bool ttl){
    TimeTTL timettl;
    timettl.ttl = ttl;
    gettimeofday(&(timettl.time), NULL);     
    data.ttls.push_back(timettl);
};

void rfir::module::ttl::SnifferV2::sniffDelta(){
    if (sniffStarted()) {
    // while (sniffStarted() && data.ttls.size() > 0) {
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

void rfir::module::ttl::SnifferV2::_sniffDelta(TimeTTL ttl){
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

std::string  rfir::module::ttl::SnifferV2::toString(){
    return data.toString();
};

void* rfir::module::ttl::SnifferV2::onPinInterrupt(void* arg, void* p) {
    auto pinEvent = (rfir::util::Interrupt::PinEvent*)p;
    this->pushTTL(pinEvent->value);
    return 0;
};

// IRAM_ATTR void rfir::module::ttl::SnifferV2::OnGpio_interrupt(void* arg) {
//     auto _this = (SnifferV2*)arg;
//     bool value = !!digitalRead(_this->params.pin);
//     if (_this->params.inverted)
//         value = !value;
//     _this->pushTTL(value);
// };

rfir::module::ttl::SnifferV2 GTTLSniffer;
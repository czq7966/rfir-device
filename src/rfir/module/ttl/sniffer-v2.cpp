#include "sniffer-v2.h"

std::string rfir::module::ttl::SnifferV2::Params::toString(){
    char result[128] = {0};
    sprintf(result, "pin: %d, mode: %d, inverted: %d, minCount: %d, maxCount: %d, minDelta: %d, maxDelta: %d, bufSize: %d",
            pin, mode, inverted, minCount, maxCount, minDelta, maxDelta);
    return result;
};

void rfir::module::ttl::SnifferV2::start(){

};
void rfir::module::ttl::SnifferV2::loop(){
    sniffDelta();
};

void rfir::module::ttl::SnifferV2::startSniff() {
    if (params.pin > 0) {
        stopSniff();
        attachInterruptArg(params.pin, OnGpio_interrupt, this, CHANGE);
        m_started = true;
        DEBUGER.println(("start sniff......: (" + this->name + ")" + params.toString()).c_str());
    } else {
        DEBUGER.println(("start sniff failed, pin is 0: (" + this->name + ")" +  params.toString()).c_str());
    }
};

void rfir::module::ttl::SnifferV2::sniffDelta(){
    while (sniffStarted() && data.ttls.size() > 0) {
        auto ttl =  data.ttls.front();
        data.ttls.pop_front();
        if (!timerisset(&data.first_ttl.time)) {
            data.first_ttl = ttl;
            data.last_ttl = ttl;
        } else if (ttl.ttl != data.last_ttl.ttl) {
            timeval delta;
            timersub(&ttl.time, &data.last_ttl.time, &delta);
            if (delta.tv_sec > 0 || delta.tv_usec >= UINT16_MAX) {
                delta.tv_sec = 0;
                delta.tv_usec = UINT16_MAX;
            }
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
        }
    }
};

std::string  rfir::module::ttl::SnifferV2::toString(){
    String str;
    if (data.deltas.size() > 0) {
        str = String(data.first_ttl.ttl) + ",";
        for (auto it = data.deltas.begin(); it != data.deltas.end(); it++)
        {        
            str = str + String(*it) + ",";
        }
    }

    return str.c_str();
};

IRAM_ATTR void rfir::module::ttl::SnifferV2::OnGpio_interrupt(void* arg) {
    auto _this = (SnifferV2*)arg;
    bool value = !!digitalRead(_this->params.pin);
    if (_this->params.inverted)
        value = !value;
    _this->pushTTL(value);
};


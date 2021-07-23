#include "sniffer.h"
#include <sys/time.h>

rfir::module::ttl::Sniffer::Sniffer() {
    new (this) Sniffer(this->params);
}

rfir::module::ttl::Sniffer::Sniffer(Params params) {
    this->init(params);    
}

rfir::module::ttl::Sniffer::~Sniffer() {
    this->uninit();
}

void rfir::module::ttl::Sniffer::init(Params params) {
    this->uninit();
    this->params = params;
    this->dataBuf = new Data[this->params.bufSize];
    this->sniffedDelta = new Delta[this->params.bufSize];

    this->pushCount = 0;
    this->popCount = 0;
    this->sniffedCount = 0;
    this->preTTL = {};

    this->gpio.init(this->params.pin, this->params.mode, this->params.inverted);
}

void rfir::module::ttl::Sniffer::uninit() {
    if (this->dataBuf)
        delete(this->dataBuf);
    if (this->sniffedDelta)
        delete(this->sniffedDelta);
    this->dataBuf = 0;
    this->sniffedDelta = 0;
}

uint32_t rfir::module::ttl::Sniffer::pushTTL(bool ttl) {
    if (pushCount - popCount + 1 >= this->params.bufSize) return 0;

    int pushPointer = pushCount % this->params.bufSize;
    this->dataBuf[pushPointer].ttl = ttl;
    gettimeofday(&(this->dataBuf[pushPointer].time), NULL);
    pushCount++;  

    return pushCount;
}


uint32_t rfir::module::ttl::Sniffer::popTTL(Data* ttl) {
    if (popCount < pushCount) {
        int popPointer = popCount % this->params.bufSize;
        ttl->ttl = this->dataBuf[popPointer].ttl;
        ttl->time = this->dataBuf[popPointer].time;
        popCount++;
        return popCount;

    } 

    return 0;
}

bool rfir::module::ttl::Sniffer::getTTL(uint32_t idx , Data* ttl) {
    idx = idx % this->params.bufSize;
    ttl->ttl = this->dataBuf[idx].ttl;
    ttl->time = this->dataBuf[idx].time;
    return true;        
}

bool rfir::module::ttl::Sniffer::getTTLDelta(Data* ttl) {
     if (pushCount < 2) return false;
    //  if ((popCount == 0 && popTTL(&preTTL) == 0 && preTTL.ttl == 0) ||
    //      (preTTL.time.tv_sec == 0 && preTTL.time.tv_usec == 0 && popTTL(&preTTL) == 0 )) {
    //        preTTL.time.tv_sec = 0;
    //        preTTL.time.tv_usec == 0;
    //        return false;
    //   }

     if (((popCount == 0 &&  preTTL.ttl == 0) ||
          (preTTL.time.tv_sec == 0 && preTTL.time.tv_usec == 0)) ) {
              if (popTTL(&preTTL) == 0) {
                preTTL.time.tv_sec = 0;
                preTTL.time.tv_usec == 0;
                return false;
              } else {
                  preMicros = micros();
              }           
      }

     if (preMicros == 0) preMicros = micros();

     Data currTTL;
     if (popTTL(&currTTL) > 0) {
         bool result = getTTLDelta(&preTTL, &currTTL, ttl);
         preTTL = currTTL;
         preMicros = micros();
         return result;
     } else {
        if (micros() - preMicros > UINT16_MAX &&  (preTTL.time.tv_sec != 0 ||  ttl->time.tv_usec != 0) ) {
        //if (micros() - preMicros > UINT16_MAX ) {
          currTTL.ttl = !(preTTL.ttl);
          preTTL.ttl = 0;
          preTTL.time.tv_sec = 0;
          preTTL.time.tv_usec = 0;
          ttl->time.tv_sec = 0;
          ttl->time.tv_usec = UINT16_MAX;
          return true;          
        }
     }
     return false;

}

bool rfir::module::ttl::Sniffer::getTTLDelta(Data* first, Data* second, Data* ttl) {
    if (first->ttl == second->ttl) 
        return false;

    ttl->ttl = first->ttl;
    timersub(&(second->time), &(first->time), &(ttl->time));

    return true;

}

int rfir::module::ttl::Sniffer::sniffDelta() {
    Data ttl;
    while (!sniffStopped && getTTLDelta(&ttl)) {
        if (ttl.time.tv_usec < this->params.minDelta || ttl.time.tv_usec > this->params.maxDelta || ttl.time.tv_usec == UINT16_MAX) {
            sniffedDelta[sniffedCount++].delta = ttl.time.tv_usec;
            if (sniffedCount >= this->params.minCount) {
                if (onSniffed)
                    onSniffed(this, getSniffedDelta(), getSniffedCount());
                return sniffedCount;
            }     
            sniffedCount = 0;       
        } else {
            if (sniffedCount == 0) sniffedDelta[sniffedCount++].delta = ttl.ttl ? 1 : 0;
            sniffedDelta[sniffedCount++].delta = ttl.time.tv_usec;
            if (sniffedCount >= this->getBufSize() - 1) {
                if (onSniffed)
                    onSniffed(this, getSniffedDelta(), getSniffedCount());                
                return sniffedCount;
            }
        }
    }

    return 0;
}

void rfir::module::ttl::Sniffer::resumeSniff() {
    sniffedCount = 0;
    sniffStopped = false;
}

void rfir::module::ttl::Sniffer::stopSniff() {
    sniffStopped = true;
}

void rfir::module::ttl::Sniffer::resetSniff() {
    sniffedCount = 0;
    sniffStopped = false;
}

String rfir::module::ttl::Sniffer::toString() {
    String str;
    int count = getSniffedCount();
    auto data = this->getSniffedDelta();    
    for (size_t i = 0; i < count; i++)
    {
        str = str + String(data[i].delta) + ",";
    } 
    return str;
}

bool rfir::module::ttl::Sniffer::parseParams(neb::CJsonObject* jp, rfir::module::ttl::Sniffer::Params* p) {
    if (jp && p) {
        int pin, mode, minCount, minDelta, maxDelta, bufSize;
        int inverted;
        if (jp->Get("pin", pin)) p->pin = pin;
        if (jp->Get("mode", mode)) p->mode = mode;
        if (jp->Get("inverted", inverted)) p->inverted = inverted;
        if (jp->Get("minCount", minCount)) p->minCount = minCount;
        if (jp->Get("minDelta", minDelta)) p->minDelta = minDelta;
        if (jp->Get("maxDelta", maxDelta)) p->maxDelta = maxDelta;
        if (jp->Get("bufSize", bufSize))   p->bufSize = bufSize;    
        return true;
    }

    return false;
}

std::string rfir::module::ttl::Sniffer::packSniffedCmd(Params* p, const char* data) {
    neb::CJsonObject jp, hd, pld, sniff, params;
    hd.Add("cmd", 9);
    hd.Add("stp", 1);

    params.Parse(p->toString());
    sniff.Add("params", params);
    sniff.Add("raw", data);
    pld.Add("sniff", sniff);

    
    jp.Add("hd", hd);
    jp.Add("pld", pld);
    return jp.ToString();    
}

std::string  rfir::module::ttl::Sniffer::Params::toString() {
  neb::CJsonObject json;
  json.Add("pin", pin);
  json.Add("mode", mode);
  json.Add("inverted", inverted);
  json.Add("minCount", minCount);
  json.Add("minDelta", minDelta);
  json.Add("maxDelta", maxDelta);
  json.Add("bufSize", bufSize);
  return json.ToString();

}


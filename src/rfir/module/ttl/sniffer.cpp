#include "rfir.h"
#include <sys/time.h>


rfir::module::ttl::Sniffer::Sniffer(RFIR* rfir) {
    new (this) Sniffer(rfir, this->sniffParams);
}

rfir::module::ttl::Sniffer::Sniffer(RFIR* rfir, SniffParams params) {
    this->rfir = rfir;
    this->gpio.setArg(this);
}

rfir::module::ttl::Sniffer::~Sniffer() {
    this->uninit();
}

void rfir::module::ttl::Sniffer::init(SniffParams params) {
    this->uninit();
    this->sniffParams = params;

    auto p = this->getSniffParams()->params;
    this->dataBuf = new Data[p.bufSize];
    this->sniffedDelta = new Delta[p.bufSize];

    if (!this->dataBuf || !this->sniffedDelta) {
        DEBUGER.println(("Sniff data buffer is too much: " + String(p.bufSize)).c_str());
    }

    this->pushCount = 0;
    this->popCount = 0;
    this->sniffedCount = 0;
    this->preTTL = {};

    this->gpio.init(p.pin, p.mode, p.inverted);
}

void rfir::module::ttl::Sniffer::uninit() {
    stop();
    if (this->dataBuf)
        delete(this->dataBuf);
    if (this->sniffedDelta)
        delete(this->sniffedDelta);
    this->dataBuf = 0;
    this->sniffedDelta = 0;
}

void rfir::module::ttl::Sniffer::start() {
    if (!sniffStopped && !this->gpio.started()) {
        if (this->gpio.pin > 0) {
            this->gpio.onChange = onGpioChange;
            this->gpio.start();
            DEBUGER.println(("start sniff......: (" + this->name + ")" + this->getSniffParams()->toString()).c_str());
        } else {
            sniffStopped = true;
            DEBUGER.println(("start sniff failed, pin is 0: (" + this->name + ")" + this->getSniffParams()->toString()).c_str());
        }
    }
}

void rfir::module::ttl::Sniffer::stop() {
    this->gpio.stop();
    this->gpio.onChange = 0;
    DEBUGER.println(("stopped sniff (" + this->name + ")").c_str());
}

bool rfir::module::ttl::Sniffer::started() {
    return this->gpio.started();
}

uint32_t rfir::module::ttl::Sniffer::pushTTL(bool ttl) {
    auto p = this->getSniffParams()->params;
    if (pushCount - popCount + 1 >= p.bufSize) return 0;

    int pushPointer = pushCount % p.bufSize;

    this->dataBuf[pushPointer].ttl = ttl;
    gettimeofday(&(this->dataBuf[pushPointer].time), NULL);
    pushCount++;  

    return pushCount;
}

rfir::module::ttl::Sniffer::SniffParams* rfir::module::ttl::Sniffer::getSniffParams() {
    return &this->sniffParams;
}

uint32_t rfir::module::ttl::Sniffer::popTTL(Data* ttl) {
    if (popCount < pushCount) {
        int popPointer = popCount % this->getSniffParams()->params.bufSize;
        ttl->ttl = this->dataBuf[popPointer].ttl;
        ttl->time = this->dataBuf[popPointer].time;
        popCount++;
        return popCount;

    } 

    return 0;
}

bool rfir::module::ttl::Sniffer::getTTL(uint32_t idx , Data* ttl) {
    idx = idx % this->getSniffParams()->params.bufSize;
    ttl->ttl = this->dataBuf[idx].ttl;
    ttl->time = this->dataBuf[idx].time;
    return true;        
}

bool rfir::module::ttl::Sniffer::getTTLDelta(Data* ttl) {
     if (pushCount < 2) return false;
    //  if ((popCount == 0 && popTTL(&preTTL) == 0 && preTTL.ttl == 0) ||
    //      (preTTL.time.tv_sec == 0 && preTTL.time.tv_usec == 0 && popTTL(&preTTL) == 0 )) {
    //        preTTL.time.tv_sec = 0;
    //        preTTL.time.tv_usec = 0;
    //        return false;
    //   }

     if (((popCount == 0 &&  preTTL.ttl == 0) ||
          (preTTL.time.tv_sec == 0 && preTTL.time.tv_usec == 0)) ) {
              if (popTTL(&preTTL) == 0) {
                preTTL.time.tv_sec = 0;
                preTTL.time.tv_usec = 0;
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
        auto p = this->getSniffParams()->params;
        if (ttl.time.tv_usec < p.minDelta || ttl.time.tv_usec > p.maxDelta || ttl.time.tv_usec == UINT16_MAX) {
            sniffedDelta[sniffedCount++].delta = ttl.time.tv_usec;
            if (sniffedCount >= p.minCount) {
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

int  rfir::module::ttl::Sniffer::getBufSize() {
    return this->getSniffParams()->params.bufSize;
}
rfir::module::ttl::Delta* rfir::module::ttl::Sniffer::getSniffedDelta() {
    return this->sniffedDelta;
}
int  rfir::module::ttl::Sniffer::getSniffedCount() {
    return (int)(this->sniffedCount);
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

void rfir::module::ttl::Sniffer::onGpioChange(Gpio* gpio, int value) {
    ((Sniffer*)gpio->getArg())->pushTTL(value);
}

std::string rfir::module::ttl::Sniffer::packSniffedCmd(Sniffer* sniffer, const char* data) {
    neb::CJsonObject jp, hd, pld, sniff, params;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", 9);
    hd.Add("stp", 1);

    params.Parse(sniffer->getSniffParams()->toString());
    sniff.Add("params", params);
    sniff.Add("raw", data);

    pld.Add("device", sniffer->name);
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

 bool rfir::module::ttl::Sniffer::Params::parseFromJson(neb::CJsonObject* jp) {
    if (jp) {
        int    pin = 22;
        int    mode = INPUT_PULLUP;
        int    inverted = false;
        int    minCount = 50;
        int    minDelta = 150;
        int    maxDelta = 15000;
        int    bufSize = 4 * 1024;       


        if (jp->Get("pin", pin)) this->pin = pin;
        if (jp->Get("mode", mode)) this->mode = mode;
        if (jp->Get("inverted", inverted)) this->inverted = inverted;
        if (jp->Get("minCount", minCount)) this->minCount = minCount;
        if (jp->Get("minDelta", minDelta)) this->minDelta = minDelta;
        if (jp->Get("maxDelta", maxDelta)) this->maxDelta = maxDelta;
        if (jp->Get("bufSize", bufSize))   this->bufSize = bufSize;    
        return true;
    }

    return false;     
 }

 std::string rfir::module::ttl::Sniffer::SniffParams::toString() {
     return this->params.toString();
 }

 bool rfir::module::ttl::Sniffer::SniffParams::parseFromJson(neb::CJsonObject* jsniff) {
    neb::CJsonObject jp;
    int response = 0;
    if (jsniff && jsniff->Get("response", response)) {    
        this->response = response;     
    }

    if (jsniff && jsniff->Get("params", jp)) {    
        return this->params.parseFromJson(&jp);
    }

    return false;
 }

 bool rfir::module::ttl::Sniffer::SniffParams::clone(SniffParams* p) {
     this->params = p->params;
     this->response = p->response;
     return true;
 }
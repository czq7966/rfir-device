#include "ttl-filter.h"
#include <sys/time.h>

rfir::receiver::TTLFilter::TTLFilter() {
    this->dataBuf = new TTLData[this->bufSize];

}

rfir::receiver::TTLFilter::~TTLFilter() {
    delete(this->dataBuf);
}

bool rfir::receiver::TTLFilter::pushTTL(bool ttl) {
    if (pushCount - popCount == this->bufSize) return 0;

    pushCount++;  
    int pushPointer = pushCount % this->bufSize;
    if (pushPointer == 0) pushPointer = this->bufSize;
    pushPointer--;
    this->dataBuf[pushPointer].ttl = ttl;
    gettimeofday(&(this->dataBuf[pushPointer].time), NULL);
    
    currTTLCount++;
    if (currTTLCount > 1) {
        auto currTime = &(this->dataBuf[pushPointer].time);
        auto preTime = &(this->dataBuf[pushPointer - 1].time);
        timeval tmSub, tmMin, tmCmp;
        tmMin.tv_sec = 0;
        tmMin.tv_usec = minTTLTime;
        timersub(currTime, preTime, &tmSub);
        time.tv_sec = currTTLCount;
        if (timercmp(&tmSub, &tmMin, <)) {
            if (currTTLCount >= minTTLCount) {
                currTime->tv_sec = 0;
                currTime->tv_usec = 0;
                stashCount = pushCount;
            }            

            pushCount = stashCount;
            currTTLCount = 0;
        } else {
            if (currTTLCount >= minTTLCount) {
                stashCount = pushCount;
            }
        }

    }

    return 1;
}


bool rfir::receiver::TTLFilter::popTTL(TTLData* ttl) {
    if (popCount < stashCount) {
        popCount++;
        int popPointer = popCount % this->bufSize;
        if (popPointer == 0) popPointer = this->bufSize;
        popPointer--;
        ttl->ttl = this->dataBuf[popPointer].ttl;
        ttl->time = this->dataBuf[popPointer].time;
        return 1;
    } 

    return 0;
}


bool rfir::receiver::TTLFilter::getTTLElapse(TTLData ttl, TTLData* data) {
    if (preElapseTTL.time.tv_sec == 0 && preElapseTTL.time.tv_usec == 0) {
        preElapseTTL = ttl;
        return 0;
    } else {
        if ((ttl.time.tv_sec == 0 && ttl.time.tv_usec == 0)) {
            preElapseTTL = {};
            return 0;
        }

        if ((ttl.ttl == preElapseTTL.ttl)) {
            return 0;
        }

        data->ttl = preElapseTTL.ttl;
        timersub(&ttl.time, &preElapseTTL.time, &(data->time));
        preElapseTTL = ttl;
        return 1;

    }

    return 0;
}
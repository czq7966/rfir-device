#include "timer.h"

rfir::util::Timer::Timer(){
    reset();
}


void rfir::util::Timer::reset() {
    start = micros();
}

uint32_t rfir::util::Timer::elapsed() {
    uint32_t now = micros();
    if (start <= now)      // Check if the system timer has wrapped.
        return now - start;  // No wrap.
    else
        return UINT32_MAX - start + now;  // Has wrapped.
}

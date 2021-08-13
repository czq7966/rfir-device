#include "timer.h"
#include <Arduino.h>


rfir::util::Timer::Timer() { reset(); }


void rfir::util::Timer::reset() {
  start = micros();
}


uint32_t rfir::util::Timer::elapsed() {
  uint32_t now = micros();
  if (start <= now) 
    return now - start; 
  else
    return UINT32_MAX - start + now; 
}

rfir::util::TimerMs::TimerMs() { reset(); }

void rfir::util::TimerMs::reset() {
  start = millis();
}

uint32_t rfir::util::TimerMs::TimerMs::elapsed() {
  uint32_t now = millis();

  if (start <= now)   
    return now - start; 
  else
    return UINT32_MAX - start + now; 
}


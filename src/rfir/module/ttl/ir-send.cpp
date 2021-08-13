#include "ir-send.h"
#include <Arduino.h>
#include <algorithm>
#include "rfir/util/timer.h"


rfir::module::ttl::IRSend::IRSend(uint16_t IRsendPin, bool inverted, bool use_modulation)
    : pin(IRsendPin), periodOffset(PeriodOffset) {
  if (inverted) {
    outputOn = LOW;
    outputOff = HIGH;
  } else {
    outputOn = HIGH;
    outputOff = LOW;
  }
  modulation = use_modulation;
  if (modulation)
    _dutycycle = DutyDefault;
  else
    _dutycycle = DutyMax;
}

void rfir::module::ttl::IRSend::begin() {
  pinMode(pin, OUTPUT);
  ledOff(); 
}

/// Turn off the IR LED.
void rfir::module::ttl::IRSend::ledOff() {
  digitalWrite(pin, outputOff);
}

void rfir::module::ttl::IRSend::ledOn() {
  digitalWrite(pin, outputOn);
}


uint32_t rfir::module::ttl::IRSend::calcUSecPeriod(uint32_t hz, bool use_offset) {
  if (hz == 0) hz = 1;  
  uint32_t period = (1000000UL + hz / 2) / hz;  
  if (use_offset)
    return std::max((uint32_t)1, period + periodOffset);
  else
    return std::max((uint32_t)1, period);
}

void rfir::module::ttl::IRSend::enableIROut(uint32_t freq, uint8_t duty) {
  if (modulation) {
    _dutycycle = std::min(duty, DutyMax);
  } else {
    _dutycycle = DutyMax;
  }
  if (freq < 1000)
    freq *= 1000;

  uint32_t period = calcUSecPeriod(freq);
  onTimePeriod = (period * _dutycycle) / DutyMax;
  offTimePeriod = period - onTimePeriod;
}

#if ALLOW_DELAY_CALLS
void rfir::module::ttl::IRsend::_delayMicroseconds(uint32_t usec) {
  if (usec <= kMaxAccurateUsecDelay) {
    delayMicroseconds(usec);
  } else {
    delay(usec / 1000UL);  
    delayMicroseconds(static_cast<uint16_t>(usec % 1000UL));
  }
}
#else  // ALLOW_DELAY_CALLS

void rfir::module::ttl::IRSend::_delayMicroseconds(uint32_t usec) {
  for (; usec > MaxAccurateUsecDelay; usec -= MaxAccurateUsecDelay)
    delayMicroseconds(MaxAccurateUsecDelay);

  delayMicroseconds(static_cast<uint16_t>(usec));
}
#endif  // ALLOW_DELAY_CALLS


uint16_t rfir::module::ttl::IRSend::mark(uint16_t usec) {
  if (!modulation || _dutycycle >= 100) {
    ledOn();
    _delayMicroseconds(usec);
    ledOff();
    return 1;
  }

  uint16_t counter = 0;
  rfir::util::Timer usecTimer = rfir::util::Timer();
  uint32_t elapsed = usecTimer.elapsed();

  while (elapsed < usec) { 
    ledOn();
    _delayMicroseconds(std::min((uint32_t)onTimePeriod, usec - elapsed));
    ledOff();
    counter++;
    if (elapsed + onTimePeriod >= usec)
      return counter; 

    _delayMicroseconds(
        std::min(usec - elapsed - onTimePeriod, (uint32_t)offTimePeriod));

    elapsed = usecTimer.elapsed();  
  }
  return counter;
}


void rfir::module::ttl::IRSend::space(uint32_t time) {
  ledOff();
  if (time == 0) return;
  _delayMicroseconds(time);
}


int8_t rfir::module::ttl::IRSend::calibrate(uint16_t hz) {
  if (hz < 1000)  
    hz *= 1000;
  periodOffset = 0;  
  enableIROut(hz);
  rfir::util::Timer usecTimer = rfir::util::Timer(); 
  uint16_t pulses = mark(UINT16_MAX); 
  uint32_t timeTaken = usecTimer.elapsed();  

  pulses = std::max(pulses, (uint16_t)1U);
  uint32_t calcPeriod = calcUSecPeriod(hz);  

  double_t actualPeriod = (double_t)timeTaken / (double_t)pulses;

  periodOffset = (int8_t)((double_t)calcPeriod - actualPeriod);
  return periodOffset;
}


void rfir::module::ttl::IRSend::sendRaw(const uint16_t buf[], const uint16_t len,
                     const uint16_t hz) {

  enableIROut(hz);
  for (uint16_t i = 0; i < len; i++) {
    if (i & 1) { 
      space(buf[i]);
    } else {  
      mark(buf[i]);
    }
  }
  ledOff();  
}

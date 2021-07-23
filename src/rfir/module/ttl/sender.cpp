#include "sender.h"
#include <sys/time.h>

rfir::module::ttl::Sender::Sender() {
    new (this) Sender(this->params);
}

rfir::module::ttl::Sender::Sender(Params params) {
    this->init(params);    
}

rfir::module::ttl::Sender::~Sender() {
    this->uninit();
}

void rfir::module::ttl::Sender::init(Params params) {
    this->uninit();
    this->params = params;
    this->gpio.init(this->params.pin, OUTPUT, this->params.inverted);
    ledOff();
}

void rfir::module::ttl::Sender::uninit() {

}

void rfir::module::ttl::Sender::enableIROut(uint32_t freq, uint8_t duty) {
    if (this->params.modulation) {
        _dutycycle = std::min(duty, (uint8_t)100);
    } else {
        _dutycycle = 100;
    }
    if (this->params.frequency < 1000)  
        freq *= 1000;

    uint32_t period = calcUSecPeriod(freq);
    onTimePeriod = (period * _dutycycle) / 100;
    offTimePeriod = period - onTimePeriod;    

}

uint32_t rfir::module::ttl::Sender::calcUSecPeriod(uint32_t hz, bool use_offset) {
    if (hz == 0) hz = 1;  // Avoid Zero hz. Divide by Zero is nasty.
    uint32_t period =
        (1000000UL + hz / 2) / hz;  // The equiv of round(1000000/hz).
    // Apply the offset and ensure we don't result in a <= 0 value.
    if (use_offset)
        return std::max((uint32_t)1, period + periodOffset);
    else
        return std::max((uint32_t)1, period);
}

void rfir::module::ttl::Sender::_delayMicroseconds(uint32_t usec) {
    delay(usec / 1000UL);
    delayMicroseconds(static_cast<uint16_t>(usec % 1000UL));    
}

uint16_t rfir::module::ttl::Sender::mark(uint16_t usec) {
    if (!this->params.modulation || _dutycycle >= 100) {
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


void rfir::module::ttl::Sender::space(uint32_t time) {
    ledOff();
    if (time == 0) return;
    _delayMicroseconds(time);
}

void rfir::module::ttl::Sender::ledOff() {
    gpio.off();
}
void rfir::module::ttl::Sender::ledOn() {
    gpio.on();
}

int8_t rfir::module::ttl::Sender::calibrate(uint16_t hz) {
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

void rfir::module::ttl::Sender::sendRaw(const uint16_t buf[], const uint16_t len, const uint16_t hz) {
    enableIROut(hz);
    for (uint16_t i = 0; i < len; i++) {
        if (i & 1) {  // Odd bit.
            space(buf[i]);
        } else {  // Even bit.
            mark(buf[i]);
        }
    }
    ledOff();     
}

void  rfir::module::ttl::Sender::sendRaw(const char* data) {


}

void rfir::module::ttl::Sender::sendData(uint16_t onemark, uint32_t onespace, uint16_t zeromark,
                                        uint32_t zerospace, uint64_t data, uint16_t nbits,
                                        bool MSBfirst ) {
    if (nbits == 0)  
        return;
    if (MSBfirst) {  
        // Send 0's until we get down to a bit size we can actually manage.
        while (nbits > sizeof(data) * 8) {
            mark(zeromark);
            space(zerospace);
            nbits--;
        }
        // Send the supplied data.
        for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
        if (data & mask) {  // Send a 1
            mark(onemark);
            space(onespace);
        } else {  // Send a 0
            mark(zeromark);
            space(zerospace);
        }
    } else {  // Send the Least Significant Bit (LSB) first / MSB last.
        for (uint16_t bit = 0; bit < nbits; bit++, data >>= 1)
        if (data & 1) {  // Send a 1
            mark(onemark);
            space(onespace);
        } else {  // Send a 0
            mark(zeromark);
            space(zerospace);
        }
    }                                            

}

void rfir::module::ttl::Sender::sendData(uint64_t data, uint16_t nbits) {
    sendData(params.onemark, params.onespace, params.zeromark, params.zerospace, data, nbits, params.MSBfirst);
}

void rfir::module::ttl::Sender::sendData(const uint8_t *dataptr, const uint16_t nbytes) {
    for (uint16_t i = 0; i < nbytes; i++)
        sendData(params.onemark, params.onespace, params.zeromark, params.zerospace, *(dataptr + i), 8, params.MSBfirst);        
}

void rfir::module::ttl::Sender::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                            const uint16_t onemark, const uint32_t onespace,
                                            const uint16_t zeromark, const uint32_t zerospace,
                                            const uint16_t footermark, const uint32_t gap,
                                            const uint64_t data, const uint16_t nbits,
                                            const uint16_t frequency, const bool MSBfirst,
                                            const uint16_t repeat, const uint8_t dutycycle) {

  sendGeneric(headermark, headerspace, onemark, onespace, zeromark, zerospace,
              footermark, gap, 0U, data, nbits, frequency, MSBfirst, repeat,
              dutycycle);
}

void rfir::module::ttl::Sender::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                            const uint16_t onemark, const uint32_t onespace,
                                            const uint16_t zeromark, const uint32_t zerospace,
                                            const uint16_t footermark, const uint32_t gap,
                                            const uint32_t mesgtime, const uint64_t data,
                                            const uint16_t nbits, const uint16_t frequency,
                                            const bool MSBfirst, const uint16_t repeat,
                                            const uint8_t dutycycle) {
    // Setup
    enableIROut(frequency, dutycycle);
    rfir::util::Timer usecs = rfir::util::Timer();

    // We always send a message, even for repeat=0, hence '<= repeat'.
    for (uint16_t r = 0; r <= repeat; r++) {
        usecs.reset();

        // Header
        if (headermark) mark(headermark);
        if (headerspace) space(headerspace);

        // Data
        sendData(onemark, onespace, zeromark, zerospace, data, nbits, MSBfirst);

        // Footer
        if (footermark) mark(footermark);
        uint32_t elapsed = usecs.elapsed();
        // Avoid potential unsigned integer underflow. e.g. when mesgtime is 0.
        if (elapsed >= mesgtime)
            space(gap);
        else
            space(std::max(gap, mesgtime - elapsed));
    }
}


void rfir::module::ttl::Sender::sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                            const uint16_t onemark, const uint32_t onespace,
                                            const uint16_t zeromark, const uint32_t zerospace,
                                            const uint16_t footermark, const uint32_t gap,
                                            const uint8_t *dataptr, const uint16_t nbytes,
                                            const uint16_t frequency, const bool MSBfirst,
                                            const uint16_t repeat, const uint8_t dutycycle) {

    // Setup
    enableIROut(frequency, dutycycle);
    // We always send a message, even for repeat=0, hence '<= repeat'.
    for (uint16_t r = 0; r <= repeat; r++) {
        // Header
        if (headermark) mark(headermark);
        if (headerspace) space(headerspace);

        // Data
        for (uint16_t i = 0; i < nbytes; i++)
            sendData(onemark, onespace, zeromark, zerospace, *(dataptr + i), 8,
                    MSBfirst);

        // Footer
        if (footermark) mark(footermark);
            space(gap);
    }
}

void rfir::module::ttl::Sender::sendGeneric(const Params p, const uint8_t *dataptr, const uint16_t nbytes) {
    sendGeneric(p.headermark, p.headerspace, p.onemark, p.onespace, p.zeromark, 
                p.zerospace, p.footermark, p.footerspace, dataptr, nbytes, 
                p.frequency, p.MSBfirst, p.repeat, p.dutycycle);

} 

void rfir::module::ttl::Sender::sendGeneric(const Params p, const uint64_t data, const uint16_t nbits) {
    sendGeneric(p.headermark, p.headerspace, p.onemark, p.onespace, p.zeromark, 
                p.zerospace, p.footermark, p.footerspace, data, nbits, 
                p.frequency, p.MSBfirst, p.repeat, p.dutycycle);
}

void rfir::module::ttl::Sender::sendGeneric(const uint8_t *dataptr, const uint16_t nbytes) {
    sendGeneric(this->params, dataptr, nbytes);
}


void rfir::module::ttl::Sender::sendGeneric(const uint64_t data, const uint16_t nbits) {
    sendGeneric(this->params, data, nbits);
}

bool rfir::module::ttl::Sender::parseParams(neb::CJsonObject* jp, rfir::module::ttl::Sender::Params* p) {
    if (jp && p) {
        int     pin = 23;
        int     inverted = false;
        int     modulation = true;
        int     repeat = 0;
        int     MSBfirst = true;
        int     frequency = 38;                    
        int     dutycycle = 50;

        int     nbits = 0;
        int     headermark = 9000;
        int     headerspace = 4500;
        int     onemark = 600;
        int     onespace = 1600;
        int     zeromark = 600;
        int     zerospace = 550;
        int     footermark = 600;
        int     footerspace = 18000;

        if (jp->Get("pin", pin))                p->pin = pin;
        if (jp->Get("inverted", inverted))      p->inverted = inverted;
        if (jp->Get("modulation", modulation))  p->modulation = modulation;
        if (jp->Get("repeat", repeat))          p->repeat = repeat;
        if (jp->Get("MSBfirst", MSBfirst))      p->MSBfirst = MSBfirst;
        if (jp->Get("frequency", frequency))    p->frequency = frequency;
        if (jp->Get("dutycycle", dutycycle))    p->dutycycle = dutycycle;    
        return true;
    }

    return false;
}


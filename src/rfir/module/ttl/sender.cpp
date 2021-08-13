#include "rfir.h"
#include <sys/time.h>

#include "ir-send.h"

rfir::module::ttl::Sender::Sender(RFIR* rfir) {
    new (this) Sender(this->sendParams, rfir);
}

rfir::module::ttl::Sender::Sender(SendParams sendParams, RFIR* rfir) {
    this->init(sendParams);    
}

rfir::module::ttl::Sender::~Sender() {
    this->uninit();
}

void rfir::module::ttl::Sender::init(SendParams sendParams) {
    this->uninit();
    this->sendParams = sendParams;
    this->gpio.init(this->getSendParams()->params.pin, OUTPUT, this->getSendParams()->params.inverted);
    ledOff();
}

void rfir::module::ttl::Sender::uninit() {

}

rfir::module::ttl::Sender::SendParams* rfir::module::ttl::Sender::getSendParams() {
    return &this->sendParams;
}

void rfir::module::ttl::Sender::enableIROut(uint32_t freq, uint8_t duty) {
    if (this->getSendParams()->params.modulation) {
        _dutycycle = std::min(duty, (uint8_t)100);
    } else {
        _dutycycle = 100;
    }
    if (this->getSendParams()->params.frequency < 1000)  
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
    if (!this->getSendParams()->params.modulation || _dutycycle >= 100) {
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
    auto p = this->getSendParams()->params;

    IRSend irsend(p.pin, p.inverted, p.modulation);
    irsend.begin();

    for (size_t i = 0; i <= p.repeat; i++)
    {
        irsend.sendRaw(buf, len, hz);
    }
    if (onSended)
        onSended(this, buf, len);
        
}

void  rfir::module::ttl::Sender::sendRaw(const char* data, const int size) {
    int count;
    uint16_t* raw  = Encoder::parseRaw(data, size, count);        
    sendRaw(raw, count);
    delete raw;
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
    //sendData(params.onemark, params.onespace, params.zeromark, params.zerospace, data, nbits, params.MSBfirst);
}

void rfir::module::ttl::Sender::sendData(const uint8_t *dataptr, const uint16_t nbytes) {
    // for (uint16_t i = 0; i < nbytes; i++)
    //     sendData(params.onemark, params.onespace, params.zeromark, params.zerospace, *(dataptr + i), 8, params.MSBfirst);        
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
    // sendGeneric(p.headermark, p.headerspace, p.onemark, p.onespace, p.zeromark, 
    //             p.zerospace, p.footermark, p.footerspace, dataptr, nbytes, 
    //             p.frequency, p.MSBfirst, p.repeat, p.dutycycle);

} 

void rfir::module::ttl::Sender::sendGeneric(const Params p, const uint64_t data, const uint16_t nbits) {
    // sendGeneric(p.headermark, p.headerspace, p.onemark, p.onespace, p.zeromark, 
    //             p.zerospace, p.footermark, p.footerspace, data, nbits, 
    //             p.frequency, p.MSBfirst, p.repeat, p.dutycycle);
}

void rfir::module::ttl::Sender::sendGeneric(const uint8_t *dataptr, const uint16_t nbytes) {
    sendGeneric(this->getSendParams()->params, dataptr, nbytes);
}


void rfir::module::ttl::Sender::sendGeneric(const uint64_t data, const uint16_t nbits) {
    sendGeneric(this->getSendParams()->params, data, nbits);
}

std::string rfir::module::ttl::Sender::Params::toString() {
    neb::CJsonObject json;
    json.Add("pin", pin);
    json.Add("inverted", inverted);
    json.Add("modulation", modulation);
    json.Add("repeat", repeat);
    json.Add("frequency", frequency);
    json.Add("dutycycle", dutycycle);
    return json.ToString();
}

bool rfir::module::ttl::Sender::Params::parseFromJson(neb::CJsonObject* jp) {
    if (jp) {
        int     pin = 0;
        int     inverted = false;
        int     modulation = true;
        int     repeat = 0;
        int     frequency = 38;                    
        int     dutycycle = 50;

        if (jp->Get("pin", pin))                this->pin = pin;
        if (jp->Get("inverted", inverted))      this->inverted = inverted;
        if (jp->Get("modulation", modulation))  this->modulation = modulation;
        if (jp->Get("repeat", repeat))          this->repeat = repeat;
        if (jp->Get("frequency", frequency))    this->frequency = frequency;
        if (jp->Get("dutycycle", dutycycle))    this->dutycycle = dutycycle;    
        return true;
    }

    return false;
}

bool rfir::module::ttl::Sender::SendParams::parseFromJson(neb::CJsonObject* jsend) {
    neb::CJsonObject jp;
    int response = 0;
    if (jsend && jsend->Get("response", response)) {    
        this->response = response;     
    }

    if (jsend && jsend->Get("params", jp))
        return this->params.parseFromJson(&jp);

    return false;
}

bool rfir::module::ttl::Sender::SendParams::clone(SendParams* p) {
    if (p) {
        this->params = p->params;
        this->response = p->response;
        return true;
    }
    return false;
}

std::string rfir::module::ttl::Sender::SendParams::toString() {
    return this->params.toString();
}


std::string rfir::module::ttl::Sender::packSniffedCmd(Sender* sender, const uint16_t* data, const uint16_t len) {
    neb::CJsonObject jp, hd, pld, sniff, params;
    hd.Add("did", rfir::util::Util::GetChipId());
    hd.Add("cmd", 8);
    hd.Add("stp", 1);

    pld.Add("device", sender->name);
    
    jp.Add("hd", hd);
    jp.Add("pld", pld);
    return jp.ToString();      

}


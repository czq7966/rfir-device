//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_TTL_SENDER_H__
#define __RFIR_MODULE_TTL_SENDER_H__

#include "rfir/util/timer.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "gpio.hpp"
#include <list>
#include <mutex>

namespace rfir {
    namespace module {
        namespace ttl {
            class Sender {     
            public:
                struct Params {
                    uint8_t  pin = 23;
                    bool     inverted = false;
                    bool     modulation = true;
                    uint16_t repeat = 0;
                    bool     MSBfirst = true;
                    uint16_t frequency = 38;                    
                    uint8_t  dutycycle = 50;

                    uint16_t nbits = 0;
                    uint16_t headermark = 9000;
                    uint32_t headerspace = 4500;
                    uint16_t onemark = 600;
                    uint32_t onespace = 1600;
                    uint16_t zeromark = 600;
                    uint32_t zerospace = 550;
                    uint16_t footermark = 600;
                    uint32_t footerspace = 18000;
                };   

                struct SendParams
                {
                    Params params;
                };
                
            protected:       
                uint8_t outputOn;
                uint8_t outputOff;
                uint16_t onTimePeriod;
                uint16_t offTimePeriod;
                int8_t periodOffset;
                uint8_t _dutycycle;

            public:              
                Params params;
                Gpio gpio;
            public:
 
                Sender();
                Sender(Params param);
                ~Sender();

                void init(Params param);
                void uninit();
                void enableIROut(uint32_t freq = 38, uint8_t duty = 50);
                uint32_t calcUSecPeriod(uint32_t hz, bool use_offset = true);                       
                virtual void _delayMicroseconds(uint32_t usec);
                virtual uint16_t mark(uint16_t usec);
                virtual void space(uint32_t usec);
                virtual void ledOff();
                virtual void ledOn();                
                int8_t calibrate(uint16_t hz = 38000U);
                void sendRaw(const uint16_t buf[], const uint16_t len, const uint16_t hz = 38);
                void sendRaw(const char* data);
                void sendData(uint16_t onemark, uint32_t onespace, uint16_t zeromark,
                                uint32_t zerospace, uint64_t data, uint16_t nbits,
                                bool MSBfirst = true);
                void sendData(uint64_t data, uint16_t nbits);
                void sendData(const uint8_t *dataptr, const uint16_t nbytes);
                void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                const uint16_t onemark, const uint32_t onespace,
                                const uint16_t zeromark, const uint32_t zerospace,
                                const uint16_t footermark, const uint32_t gap,
                                const uint64_t data, const uint16_t nbits,
                                const uint16_t frequency, const bool MSBfirst,
                                const uint16_t repeat, const uint8_t dutycycle);
                void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                const uint16_t onemark, const uint32_t onespace,
                                const uint16_t zeromark, const uint32_t zerospace,
                                const uint16_t footermark, const uint32_t gap,
                                const uint32_t mesgtime, const uint64_t data,
                                const uint16_t nbits, const uint16_t frequency,
                                const bool MSBfirst, const uint16_t repeat,
                                const uint8_t dutycycle);
                void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                                const uint16_t onemark, const uint32_t onespace,
                                const uint16_t zeromark, const uint32_t zerospace,
                                const uint16_t footermark, const uint32_t gap,
                                const uint8_t *dataptr, const uint16_t nbytes,
                                const uint16_t frequency, const bool MSBfirst,
                                const uint16_t repeat, const uint8_t dutycycle);      
                void sendGeneric(const Params params, const uint8_t *dataptr, const uint16_t nbytes);
                void sendGeneric(const Params params, const uint64_t data, const uint16_t nbits);   
                void sendGeneric(const uint8_t *dataptr, const uint16_t nbytes);
                void sendGeneric(const uint64_t data, const uint16_t nbits);                   
            public:
                static bool parseParams(neb::CJsonObject* jp, rfir::module::ttl::Sender::Params* p);

            };
        }
    }
}


#endif //__RFIR_MODULE_TTL_SENDER_H__

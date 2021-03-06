//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_TTL_SENDER_H__
#define __RFIR_MODULE_TTL_SENDER_H__

#include "rfir/util/timer.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "encoder.h"
#include "gpio.h"
#include <list>
#include <mutex>

namespace rfir {
    namespace module {
        namespace ttl {
            class RFIR;
            class Sender {     
            public:
                typedef std::function<void(Sender* sender, const uint16_t* data, const uint16_t len)> OnSended;  
                struct Params {
                    uint8_t  pin = 0;
                    bool     inverted = false;
                    bool     modulation = true;
                    uint16_t repeat = 0;
                    uint16_t frequency = 38;                    
                    uint8_t  dutycycle = 50;

                    std::string   toString();
                    bool          parseFromJson(neb::CJsonObject* jp);
                };   

                struct SendParams
                {
                    Params params;
                    bool   response = 0;
                    bool   parseFromJson(neb::CJsonObject* jp);
                    bool   clone(SendParams* p);
                    std::string   toString();
                };
                
            protected:       
                uint8_t outputOn;
                uint8_t outputOff;
                uint16_t onTimePeriod;
                uint16_t offTimePeriod;
                int8_t periodOffset;
                uint8_t _dutycycle;

            private:              
                SendParams sendParams;
            public:
                std::string name;
                Gpio gpio;
                OnSended   onSended = 0;
            public:
 
                Sender(RFIR* rfir = 0);
                Sender(SendParams sendParams, RFIR* rfir = 0);
                ~Sender();

                void init(SendParams sendParams);
                void uninit();
                SendParams* getSendParams();


                void enableIROut(uint32_t freq = 38, uint8_t duty = 50);
                uint32_t calcUSecPeriod(uint32_t hz, bool use_offset = true);                       
                virtual void _delayMicroseconds(uint32_t usec);
                virtual uint16_t mark(uint16_t usec);
                virtual void space(uint32_t usec);
                virtual void ledOff();
                virtual void ledOn();                
                int8_t calibrate(uint16_t hz = 38000U);

                void sendRaw(const uint16_t buf[], const uint16_t len, const uint16_t hz = 38);
                void sendRaw(const char* data, const int size);
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
                static std::string packSendedCmd(Sender* sender, const uint16_t* data, const uint16_t len);
            };
        }
    }
}


#endif //__RFIR_MODULE_TTL_SENDER_H__

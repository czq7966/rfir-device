#ifndef __RFIR_MODULE_DEVICE_TEST_SNIFFER_H__
#define __RFIR_MODULE_DEVICE_TEST_SNIFFER_H__


#include "../../device.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace test {
                class Sniffer: public Device {
                public:
                    static const uint16_t KSnifferLength = 8;
                public:
                    union Protocol{
                        uint8_t remote_state[KSnifferLength];  
                    };  

                    Protocol protocol;
                public:
                    rfir::module::ttl::Config::Device* initTempCodec();                    
                    rfir::module::ttl::Config::Device* initOkonoffCodec();                    
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;                
                    virtual uint16_t* getEncodeRaw(int& count) override;
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override;                     
                };
            }
        }
    }
}



#endif //

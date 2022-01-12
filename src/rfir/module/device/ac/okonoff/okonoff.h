#ifndef __RFIR_MODULE_DEVICE_AC_OKONOFF_H__
#define __RFIR_MODULE_DEVICE_AC_OKONOFF_H__


#include "../../device.h"
#include "rfir/rfir.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Okonoff: public Device {
                public:                     
                    Okonoff();
                    ~Okonoff();
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;            
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual uint16_t* getEncodeRaw(int& count);         
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override; 
                };
            }
        }
    }
}



#endif //

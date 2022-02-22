#ifndef __RFIR_MODULE_DEVICE_AC_GREE_H__
#define __RFIR_MODULE_DEVICE_AC_GREE_H__


#include "../../device.h"
#include "rfir/rfir.h"
#include "gree-ac.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Gree: public Device {
                protected:
                    GreeAC ac;
                public:                     
                    Gree();
                    ~Gree();
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;            
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual uint16_t* getEncodeRaw(int& count);         
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override; 
                    void dump();
                };
            }
        }
    }
}



#endif //

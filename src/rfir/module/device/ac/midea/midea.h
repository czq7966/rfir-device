#ifndef __RFIR_MODULE_DEVICE_AC_MIDEA_H__
#define __RFIR_MODULE_DEVICE_AC_MIDEA_H__


#include "../../rfir-device.h"
#include "midea-ac.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Midea: public RFIRDevice {
                public:
                    ac:: MideaAC ac;
                public:
                    virtual void init() override;            
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual bool getEncodeRaw(std::list<uint16_t>& result) override;       
                    virtual bool onSvc_set(neb::CJsonObject* pld) override; 
                    virtual bool onSvc_get(neb::CJsonObject* pld) override; 
                    virtual bool onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& p) override; 
                };
            }
        }
    }
}



#endif //

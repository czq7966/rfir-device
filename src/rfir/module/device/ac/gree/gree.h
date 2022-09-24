#ifndef __RFIR_MODULE_DEVICE_AC_GREE_H__
#define __RFIR_MODULE_DEVICE_AC_GREE_H__


#include "../../rfir-device.h"
#include "gree-ac.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Gree: public RFIRDevice {
                protected:
                    GreeAC ac;
                public:
                    virtual void init() override;            
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual bool getEncodeRaw(std::list<uint16_t>& result) override;       
                    virtual int onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& p) override; 
                };
            }
        }
    }
}



#endif //

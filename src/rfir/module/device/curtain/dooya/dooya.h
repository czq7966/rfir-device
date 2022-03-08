#ifndef __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DOOYA_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DOOYA_H__

#include "../../device.h"
#include "dooya-485.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace curtain {
                class Dooya: public Device {
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;            
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                };
            }
        }
    }
}
                

#endif                

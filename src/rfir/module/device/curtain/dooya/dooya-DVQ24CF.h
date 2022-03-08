#ifndef __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DVQ24CF_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DVQ24CF_H__

#include "dooya.h"
#include "dooya-485.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace curtain {
                class Dooya_DVQ24CF: public Dooya {
                public:
                    Dooya485* d485;
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
#ifndef __RFIR_MODULE_DEVICE_AC_FZ_PMZ_F2_GREE_H__
#define __RFIR_MODULE_DEVICE_AC_FZ_PMZ_F2_GREE_H__

#include "gree.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZ_PMZ_F2_Gree: public Gree {    
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                                       
                    virtual void init() override;
                    virtual int onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:
                    static const uint8_t PIN_POWER  = 13;                              
                };
            }
        }
    }
}



#endif //

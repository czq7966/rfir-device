#ifndef __RFIR_MODULE_DEVICE_AC_FZ_DM_MIDEA_H__
#define __RFIR_MODULE_DEVICE_AC_FZ_DM_MIDEA_H__

#include "midea.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZ_DM_Midea: public Midea {              
                public:
                    virtual void start(void *) override;
                            
                    virtual void init() override;
                    virtual int onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:
                    static const uint8_t PIN_POWER  = 13;  
                };
            }
        }
    }
}



#endif //

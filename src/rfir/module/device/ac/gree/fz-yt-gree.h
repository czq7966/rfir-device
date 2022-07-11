#ifndef __RFIR_MODULE_DEVICE_AC_FZ_YT_GREE_H__
#define __RFIR_MODULE_DEVICE_AC_FZ_YT_GREE_H__

#include "gree.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZ_YT_Gree: public Gree {    
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                                       
                    virtual void init() override;
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                public:
                    static const uint8_t PIN_POWER  = 13;                              
                };
            }
        }
    }
}



#endif //

#ifndef __RFIR_MODULE_DEVICE_CURTAIN_CL_DJROOM_DOOYA_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_CL_DJROOM_DOOYA_H__


#include "../../rs/rs485.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace curtain {
                class CL_DJROOM_Dooya_485: public RS::RS485 {
                public:
                    virtual void init() override;
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                };
            }
        }
    }
}



#endif //

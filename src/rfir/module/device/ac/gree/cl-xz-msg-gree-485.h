#ifndef __RFIR_MODULE_DEVICE_AC_CL_VRHALL_MCQUAY_485_H__
#define __RFIR_MODULE_DEVICE_AC_CL_VRHALL_MCQUAY_485_H__


#include "../../rs/rs485.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_XZ_MSG_GREE_485: public RS::RS485 {
                public:
                    virtual void init() override;
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                };
            }
        }
    }
}



#endif //

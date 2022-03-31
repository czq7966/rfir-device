#ifndef __RFIR_MODULE_DEVICE_AC_CL_VRHALL_MCQUAY_485_H__
#define __RFIR_MODULE_DEVICE_AC_CL_VRHALL_MCQUAY_485_H__


#include "../../rs/rs485.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_VRHALL_F1_MCQUAY_485: public RS::RS485 {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                };
            }
        }
    }
}



#endif //

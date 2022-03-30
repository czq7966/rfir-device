#ifndef __RFIR_MODULE_DEVICE_AC_CL_DJROOM_MCQUAY_H__
#define __RFIR_MODULE_DEVICE_AC_CL_DJROOM_MCQUAY_H__


#include "../../rs/rs485.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_VRHALL_F1_MCQUAY_485: public RS::RS485 {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

#ifndef __RFIR_MODULE_DEVICE_AC_CL_DJROOM_MCQUAY_H__
#define __RFIR_MODULE_DEVICE_AC_CL_DJROOM_MCQUAY_H__


#include "mcquay.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_DJROOM_Mcquay: public Mcquay {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

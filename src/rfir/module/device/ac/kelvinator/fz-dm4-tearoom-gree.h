#ifndef __RFIR_MODULE_DEVICE_AC_FZDM4_TeaRoom_Gree_H__
#define __RFIR_MODULE_DEVICE_AC_FZDM4_TeaRoom_Gree_H__


#include "kelvinator.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZDM4_TeaRoom_Gree: public Kelvinator {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

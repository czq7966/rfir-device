#ifndef __RFIR_MODULE_DEVICE_AC_FZ_YT_MIDEA_H__
#define __RFIR_MODULE_DEVICE_AC_FZ_YT_MIDEA_H__

#include "fz-dm-midea.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZ_YT_Midea: public FZ_DM_Midea {              
                public:
                    virtual void init() override;
                };
            }
        }
    }
}



#endif //

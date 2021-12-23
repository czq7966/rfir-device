#ifndef __RFIR_MODULE_DEVICE_MSWITCH_CL_DJROOM_Light_MSwitch2_H__
#define __RFIR_MODULE_DEVICE_MSWITCH_CL_DJROOM_Light_MSwitch2_H__


#include "mswitch.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace mswitch {
                class CL_DJROOM_Light_MSwitch2: public MSwitch {
                public:
                    virtual void start(void *) override;
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

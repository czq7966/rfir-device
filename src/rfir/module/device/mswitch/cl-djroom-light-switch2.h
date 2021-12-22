#ifndef __RFIR_MODULE_DEVICE_MSWITCH_CL_DJROOM_Light_Switch2_H__
#define __RFIR_MODULE_DEVICE_MSWITCH_CL_DJROOM_Light_Switch2_H__


#include "mswitch.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace mswitch {
                class CL_DJROOM_Light_Switch2: public MSwitch {
                public:
                    virtual void start(void *) override;
                    virtual void loop() override; 
                };
            }
        }
    }
}



#endif //

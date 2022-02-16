#ifndef __RFIR_MODULE_DEVICE_AC_CL_VRHALL_F2_COOLIX_H__
#define __RFIR_MODULE_DEVICE_AC_CL_VRHALL_F2_COOLIX_H__

#include "coolix.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_VRHALL_F2_Coolix: public Coolix {      
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                                       
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

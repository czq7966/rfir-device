#ifndef __RFIR_MODULE_DEVICE_AC_CL_VRHALL_FF_OKONOFF_H__
#define __RFIR_MODULE_DEVICE_AC_CL_VRHALL_FF_OKONOFF_H__


#include "okonoff.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_VRHALL_FF_Okonoff: public Okonoff {                
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

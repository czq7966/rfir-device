#ifndef __RFIR_MODULE_DEVICE_RFIR_RFIR_PENET_IR8285_H__
#define __RFIR_MODULE_DEVICE_RFIR_RFIR_PENET_IR8285_H__


#include "rfir-penet.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace rfir{
                class RFIRPenetIR8285: public RFIRPenet {
                public:
                    virtual void init() override;   
                };
            }
        }
    }
}



#endif //

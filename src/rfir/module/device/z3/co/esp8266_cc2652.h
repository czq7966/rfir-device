#ifndef __RFIR_MODULE_DEVICE_Z3_CO_ESP8266_CC2652_H__
#define __RFIR_MODULE_DEVICE_Z3_CO_ESP8266_CC2652_H__


#include "../../device.h"
#include "coordinator.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace z3 {
                namespace co {
                    class ESP8266_CC2652: public Coordinator {
                    public:
                        virtual void start(void *) override;
                        virtual void init() override;                                
                    };

                }
            }
        }
    }
}



#endif //

#ifndef __RFIR_MODULE_DEVICE_AC_FZ_PMZ_F2_GREE_H__
#define __RFIR_MODULE_DEVICE_AC_FZ_PMZ_F2_GREE_H__

#include "gree.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class FZ_PMZ_F2_Gree: public Gree {    
                public:
                    rfir::module::ttl::Gpio    gpioPower; 
                    bool doPowerPinChange(); 
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                                       
                    virtual rfir::module::ttl::Config::Device* init() override;
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                public:
                    static const uint8_t PIN_POWER  = 13;  
                    static bool PowerPinChanged;
                    static void OnPowerPinChange(rfir::module::ttl::Gpio* gpio, int value);                               
                };
            }
        }
    }
}



#endif //

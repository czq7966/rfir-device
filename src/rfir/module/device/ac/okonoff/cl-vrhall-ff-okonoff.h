#ifndef __RFIR_MODULE_DEVICE_AC_CL_VRHALL_FF_OKONOFF_H__
#define __RFIR_MODULE_DEVICE_AC_CL_VRHALL_FF_OKONOFF_H__

#include "okonoff.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CL_VRHALL_FF_Okonoff: public Okonoff {       
                public:
                    bool onCmd_get(neb::CJsonObject* pld);
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                                       
                    virtual rfir::module::ttl::Config::Device* init() override;
                public:
                    static void On_HLW8110_IA_Switch(int arg, bool ison);
                public:
                    bool initSensor();
                public:                
                    bool waitStart(int timeout = 2000);
                    bool isRunning(float& iaValue, bool realtime = true);
                    void syncPower();
                    bool isPowerOff(bool realtime = true);
                    void powerOff();
                    int  powerOffCountdown(bool reset = false, int timeout = 22 * 1000);
                };
            }
        }
    }
}



#endif //

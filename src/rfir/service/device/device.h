#ifndef __RFIR_SERVICE_DEVICE_H__
#define __RFIR_SERVICE_DEVICE_H__


#include <Arduino.h>
#include "config-rfir.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/gpio.h"
#include "service/cmds/cmd.h"

namespace rfir {
    namespace service {
        namespace device {
            class Device {
            public:
                static rfir::module::ttl::Config::Device* Init();
                static void Start();
                static void Loop();
                static bool OnCmd(const char* cmd);
                static bool OnCmd(neb::CJsonObject* cmd);
                static void OnSetRaw(void*);
                static void SetRaw(void*);
                static bool InitRaw();      
                static void DoTimerReport(bool reset = false);          
            };
        }
    }
}



#endif //

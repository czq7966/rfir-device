#ifndef __RFIR_SERVICE_DEVICE_H__
#define __RFIR_SERVICE_DEVICE_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/device/device.h"


namespace rfir {
    namespace service {
        namespace device {
            class Device {
            public:
                static rfir::module::device::Device* GDevice;
                static rfir::module::ttl::Config::Device* Init();
                static rfir::module::ttl::Config::Device* GetConfig();
            public:
                static void Start(rfir::module::device::Device::OnChange onChange);
                static void Loop();
                static void DoTimerReport(bool reset = false);     
                static bool SendEncodeRaw();
            public:
                static bool OnCmd_set(neb::CJsonObject* pld);
                static bool OnCmd_get(neb::CJsonObject* pld);
                static bool OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data);
            };
        }
    }
}



#endif //

// #ifndef __RFIR_SERVICE_DEVICE_H__
// #define __RFIR_SERVICE_DEVICE_H__


// #include <Arduino.h>
// #include "config.h"
// #include "rfir/util/cjson/CJsonObject.hpp"
// #include "rfir/module/device/device.h"


// namespace rfir {
//     namespace service {
//         namespace device {            
//             class Device {
//             public:
//                 // static rfir::module::device::Device* SDevice;
//                 // static rfir::module::device::Networking* SNetworking;
//                 static rfir::module::ttl::Config::Device* Init();
//                 static rfir::module::ttl::Config::Device* GetConfig();
//             public:
//                 static void Start(rfir::util::Event::Callback onChange);
//                 static void Loop();
//                 static void DoTimerReport(bool reset = false);     
//                 static bool SendEncodeRaw();
//             public:
//                 //V2
//                 // static void* OnCommand(void* arg, void* p);                
//                 // static bool  OnCmd_set(cmds::cmd::CmdMqtt* cmd);
//                 // static bool  OnCmd_get(cmds::cmd::CmdMqtt* cmd, );                

//                 // static void* OnDeviceChange(void* arg, void* p);
//             public:

//                 static bool OnCmd_set(neb::CJsonObject* pld);
//                 static bool OnCmd_get(neb::CJsonObject* pld);
//                 static bool OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data);
//                 static bool OnCmd_config(neb::CJsonObject* pld);
//             public:
                
//             };

//         }
//     }
// }



// #endif //

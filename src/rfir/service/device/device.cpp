#include "device.h"
#include "cmds/cmd/cmd-dispatcher.h"

// #ifdef DEVICE_FZ_DM4_TEAROOM_AC_GREE
// #include "rfir/module/device/ac/kelvinator/fz-dm4-tearoom-gree.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::FZDM4_TeaRoom_Gree();
// #endif

// //长乐DJ会议室Mcquay空调
// #ifdef DEVICE_CL_DJROOM_AC_MCQUAY
// #include "rfir/module/device/ac/mcquay/cl-djroom-mcquay.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::CL_DJROOM_Mcquay();
// #endif

// //长乐DJ会议室杜亚窗帘
// #ifdef DEVICE_CL_DJROOM_CURTAIN_DOOYA_485
// #include "rfir/module/device/curtain/dooya/cl-djroom-dooya-485.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::curtain::CL_DJROOM_Dooya_485();
// #endif

// //长乐DJ会议室乐高区2路射灯开关
// #ifdef DEVICE_CL_DJROOM_LIGHT_MSWITCH2
// #include "rfir/module/device/mswitch/cl-djroom-light-mswitch2.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::mswitch::CL_DJROOM_Light_MSwitch2();
// #endif


// //长乐VR体验馆党建一楼柯耐弗空调风盘
// #ifdef DEVICE_CL_VRHALL_FIRST_FLOOR_AC_OKONOFF
// #include "rfir/module/device/ac/okonoff/cl-vrhall-ff-okonoff.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::CL_VRHALL_FF_Okonoff();
// #endif

// //长乐VR体验馆党建一楼室外水机McQuay
// #ifdef DEVICE_CL_VRHALL_F1_AC_MCQUAY_485
// #include "rfir/module/device/ac/mcquay/cl-vrhall-f1-ac-mcquay-485.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485();
// #endif

// //长乐VR体验馆二三楼Coolix空调
// #ifdef DEVICE_CL_VRHALL_F2_AC_COOLIX
// #include "rfir/module/device/ac/coolix/cl-vrhall-f2-coolix.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::CL_VRHALL_F2_Coolix();
// #endif

// #ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
// #include "rfir/module/device/speaker/sansui/cl-cxy-sansui-p300.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::speaker::CL_CXY_SANSUI_P300();
// #endif

// #ifdef DEVICE_TEST_SNIFFER_ESP32_WROOM
// #include "rfir/module/device/test/sniffer/esp32_wroom.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::test::Sniffer_esp32_wroom();
// #endif

// //福州飘渺庄2楼食堂格力空调
// #ifdef DEVICE_FZ_PMZ_F2_AC_GREE
// #include "rfir/module/device/ac/gree/fz-pmz-f2-gree.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::FZ_PMZ_F2_Gree();
// #endif

// //福州飘渺庄2楼食堂格力空调
// #ifdef DEVICE_FZ_DM3_AC_MIDEA
// #include "rfir/module/device/ac/midea/fz-dm3-midea.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::ac::FZ_DM3_Midea();
// #endif

// //Z3 协调器 ESP8266 + CC2652 
// #ifdef DEVICE_Z3_COORDINATOR_ESP8266_CC2652
// #include "rfir/module/device/z3/co/esp8266_cc2652.h"
// rfir::module::device::Device* rfir::service::device::Device::SDevice = new rfir::module::device::z3::co::ESP8266_CC2652();
// #endif



// rfir::module::device::Networking* rfir::service::device::Device::SNetworking = new rfir::module::device::Networking();
// rfir::module::device::Device* GDevice = rfir::service::device::Device::SDevice;

// rfir::module::ttl::Config::Device* rfir::service::device::Device::Init() {
//     return SDevice->init();
// }

// rfir::module::ttl::Config::Device* rfir::service::device::Device::GetConfig() {
//     auto ds = &rfir::RFIR::Config->devices;
//     auto d = ds->getDevice(SDevice->name);
//     return d;
// }

// void rfir::service::device::Device::Start(rfir::util::Event::Callback onChange) {
//     SDevice->start(0);

//     SDevice->events.onChange.add((void*)&onChange, onChange);
//     // GCmdDispatcher.events.onCommand.add((void*)&OnCommand, OnCommand);
// }

// void rfir::service::device::Device::Loop() {
//     SDevice->loop();

// }

// void rfir::service::device::Device::DoTimerReport(bool reset) {
//     SDevice->doTimerReport(reset);
// }

// bool rfir::service::device::Device::SendEncodeRaw() {
//     int count = 0;
//     auto raw = SDevice->getEncodeRaw(count);
//     if (raw && count) {
//         auto rfir = rfir::GetRfir();
//         rfir->sniffer->stop();
//         rfir->sender->sendRaw(raw, count);
//         rfir->sniffer->start();
//         return 1;
//     }
//     return 0;
// }


// bool rfir::service::device::Device::OnCmd_set(neb::CJsonObject* pld) {
//     auto result = SDevice->onCmd_set(pld);

//     //发码
//     if (result) SendEncodeRaw();

//     return result;
// }

// bool rfir::service::device::Device::OnCmd_get(neb::CJsonObject* pld) {
//     auto result = SDevice->getProps(pld);
//     if (result)
//         SDevice->reinitTimerReport();
//     return result;
// }

// bool rfir::service::device::Device::OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
//     return SDevice->onCmd_decoded(data);

// }

// bool rfir::service::device::Device::OnCmd_config(neb::CJsonObject* pld) {
//     return false;
// }

//V2

// void* rfir::service::device::Device::OnCommand(void* arg, void* p) {
//     auto cmd = (cmds::cmd::CmdMqtt*)p;
//     if (cmd->command.head.entry.type == "svc") {
//         if (cmd->command.head.entry.id == Config.mqtt_dev_svc_get) {
//             OnCmd_get(cmd);
//         }

//         if (cmd->command.head.entry.id == Config.mqtt_dev_svc_set) {
//             OnCmd_set(cmd);            
//         }
//     }
//     return 0;
// };


// bool  rfir::service::device::Device::OnCmd_set(cmds::cmd::CmdMqtt* cmd){
//     return 0;
// };
// bool  rfir::service::device::Device::OnCmd_get(cmds::cmd::CmdMqtt* cmd){
//     return 0;
// };     


// void* rfir::service::device::Device::OnDeviceChange(void* arg, void* p){
//     auto reason = (const char*)p;
//     DEBUGER.printf("onDeviceChange: %s\n", reason);
//     OnCmd_get((::cmds::cmd::CmdMqtt*)0, reason);
//     return 0;
// };

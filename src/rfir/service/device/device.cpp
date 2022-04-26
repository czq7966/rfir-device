#include "device.h"

#ifdef DEVICE_FZ_DM4_TEAROOM_AC_GREE
#include "rfir/module/device/ac/kelvinator/fz-dm4-tearoom-gree.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::FZDM4_TeaRoom_Gree();
#endif

//长乐DJ会议室Mcquay空调
#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "rfir/module/device/ac/mcquay/cl-djroom-mcquay.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::CL_DJROOM_Mcquay();
#endif

//长乐DJ会议室杜亚窗帘
#ifdef DEVICE_CL_DJROOM_CURTAIN_DOOYA_485
#include "rfir/module/device/curtain/dooya/cl-djroom-dooya-485.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::curtain::CL_DJROOM_Dooya_485();
#endif

//长乐DJ会议室乐高区2路射灯开关
#ifdef DEVICE_CL_DJROOM_LIGHT_MSWITCH2
#include "rfir/module/device/mswitch/cl-djroom-light-mswitch2.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::mswitch::CL_DJROOM_Light_MSwitch2();
#endif


//长乐VR体验馆党建一楼柯耐弗空调风盘
#ifdef DEVICE_CL_VRHALL_FIRST_FLOOR_AC_OKONOFF
#include "rfir/module/device/ac/okonoff/cl-vrhall-ff-okonoff.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::CL_VRHALL_FF_Okonoff();
#endif

//长乐VR体验馆党建一楼室外水机McQuay
#ifdef DEVICE_CL_VRHALL_F1_AC_MCQUAY_485
#include "rfir/module/device/ac/mcquay/cl-vrhall-f1-ac-mcquay-485.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485();
#endif

//长乐VR体验馆二三楼Coolix空调
#ifdef DEVICE_CL_VRHALL_F2_AC_COOLIX
#include "rfir/module/device/ac/coolix/cl-vrhall-f2-coolix.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::CL_VRHALL_F2_Coolix();
#endif

#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "rfir/module/device/speaker/sansui/cl-cxy-sansui-p300.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::speaker::CL_CXY_SANSUI_P300();
#endif

#ifdef DEVICE_TEST_SNIFFER_ESP32_WROOM
#include "rfir/module/device/test/sniffer/esp32_wroom.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::test::Sniffer_esp32_wroom();
#endif

//福州飘渺庄2楼食堂格力空调
#ifdef DEVICE_FZ_PMZ_F2_AC_GREE
#include "rfir/module/device/ac/gree/fz-pmz-f2-gree.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::FZ_PMZ_F2_Gree();
#endif

//福州飘渺庄2楼食堂格力空调
#ifdef DEVICE_FZ_DM3_AC_MEDIA
#include "rfir/module/device/ac/midea/fz-dm3-midea.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::FZ_DM3_Midea();
#endif

//Z3 协调器 ESP8266 + CC2652 
#ifdef DEVICE_Z3_COORDINATOR_ESP8266_CC2652
#include "rfir/module/device/z3/co/esp8266_cc2652.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::z3::co::ESP8266_CC2652();
#endif


rfir::module::device::Networking* rfir::service::device::Device::GNetworking = new rfir::module::device::Networking();
rfir::module::ttl::Config::Device* rfir::service::device::Device::Init() {
    return GDevice->init();
}

rfir::module::ttl::Config::Device* rfir::service::device::Device::GetConfig() {
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->getDevice(GDevice->name);
    return d;
}

void rfir::service::device::Device::Start(rfir::module::device::Device::OnChange onChange) {
    GDevice->start(0);
    GDevice->onChange = onChange;   
    GNetworking->start(); 
}

void rfir::service::device::Device::Loop() {
    GDevice->loop();
    GNetworking->loop(); 
}

void rfir::service::device::Device::DoTimerReport(bool reset) {
    GDevice->doTimerReport(reset);
}

bool rfir::service::device::Device::SendEncodeRaw() {
    int count = 0;
    auto raw = GDevice->getEncodeRaw(count);
    if (raw && count) {
        auto rfir = rfir::GetRfir();
        rfir->sniffer->stop();
        rfir->sender->sendRaw(raw, count);
        rfir->sniffer->start();
        return 1;
    }
    return 0;
}

bool rfir::service::device::Device::OnCmd_set(neb::CJsonObject* pld) {
    auto result = GDevice->onCmd_set(pld);

    //发码
    if (result) SendEncodeRaw();

    return result;
}

bool rfir::service::device::Device::OnCmd_get(neb::CJsonObject* pld) {
    auto result = GDevice->onCmd_get(pld);
    if (result)
        GDevice->reinitTimerReport();
    return result;
}

bool rfir::service::device::Device::OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    return GDevice->onCmd_decoded(data);

}

bool rfir::service::device::Device::OnCmd_config(neb::CJsonObject* pld) {
    return false;
}
#include "device.h"

#ifdef DEVICE_FZ_DM4_TEAROOM_AC_GREE
#include "rfir/module/device/ac/kelvinator/fz-dm4-tearoom-gree.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::FZDM4_TeaRoom_Gree();
#endif

#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "rfir/module/device/ac/mcquay/cl-djroom-mcquay.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::CL_DJROOM_Mcquay();
#endif

#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "rfir/module/device/speaker/sansui/cl-cxy-sansui-p300.h"
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::speaker::CL_CXY_SANSUI_P300();
#endif


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
}

void rfir::service::device::Device::Loop() {
    GDevice->loop();
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

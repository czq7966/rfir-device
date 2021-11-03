#include "device.h"
#include "rfir/module/device/ac/kelvinator/kelvinator.h"


#ifdef DEVICE_FZ_DM4_AC_GREE
rfir::module::device::Device* rfir::service::device::Device::GDevice = new rfir::module::device::ac::Kelvinator();
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
    return GDevice->onCmd_get(pld);
}

bool rfir::service::device::Device::OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    return GDevice->onCmd_decoded(data);

}

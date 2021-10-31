#include "device.h"
#include "ac/gree/gree.h"
#include "ac/mcquay/mcquay.h"

rfir::module::ttl::Config::Device* rfir::service::device::Device::Init() {
#ifdef RFIR_DEVICE_GREE
    return rfir::service::device::ac::Gree::Init();
#endif

#ifdef RFIR_DEVICE_MAQUAY
    return rfir::service::device::ac::Mcquay::Init();
#endif

}


void rfir::service::device::Device::Start() {

}

void rfir::service::device::Device::Loop() {

}
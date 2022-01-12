#include "okonoff.h"



rfir::module::ttl::Config::Device* rfir::module::device::ac::Okonoff::init() {
    this->name = "Okonoff";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 50;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = true;

    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 40;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 64;
    dp[0].headermark = 4690;
    dp[0].headerspace = 2610;
    dp[0].onemark = 370;
    dp[0].onespace = 910;
    dp[0].zeromark = 370;
    dp[0].zerospace = 390;
    dp[0].footermark = 370;
    dp[0].footerspace = 20470;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;    
}



rfir::module::device::ac::Okonoff::Okonoff() {


}

rfir::module::device::ac::Okonoff::~Okonoff() {

}


void rfir::module::device::ac::Okonoff::start(void * p) {
    Device::start(p);

}


void rfir::module::device::ac::Okonoff::loop() {
    Device::loop();
}

bool rfir::module::device::ac::Okonoff::setRaw(uint8_t* raw) {
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Okonoff::getRaw(int& count) {

    return 0;
}

uint16_t* rfir::module::device::ac::Okonoff::getEncodeRaw(int& count) {
    return 0;
}

bool rfir::module::device::ac::Okonoff::onCmd_set(neb::CJsonObject* pld) {

    return false;
}


bool rfir::module::device::ac::Okonoff::onCmd_get(neb::CJsonObject* pld) {
    return false;
}

bool rfir::module::device::ac::Okonoff::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count >= 1) {
        int size = 0;
        auto raw = getRaw(size);
        memcpy(raw, data->result[0].bytes, size);;
        return setRaw(raw);
    }
    return false;
}

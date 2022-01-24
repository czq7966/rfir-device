#include "sniffer.h"


rfir::module::ttl::Config::Device* rfir::module::device::test::Sniffer::init() {
   this->name = "sniffer";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 50;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = false;

    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 20;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 32;
    dp[0].headermark = 3500;
    dp[0].headerspace = 1700;
    dp[0].onemark = 560;
    dp[0].onespace = 1150;
    dp[0].zeromark = 560;
    dp[0].zerospace = 340;
    dp[0].footermark = 0;
    dp[0].footerspace = 0;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;    
}

bool rfir::module::device::test::Sniffer::setRaw(uint8_t* raw) {
    memcpy(protocol.remote_state, raw, KSnifferLength);
    this->onSetRaw();
    return true;
};

uint8_t* rfir::module::device::test::Sniffer::getRaw(int& count) {
    count = KSnifferLength;
    return this->protocol.remote_state;

};
           
uint16_t* rfir::module::device::test::Sniffer::getEncodeRaw(int& count) {
    int length = 0;
    auto raw = getRaw(length);
    auto str1 = rfir::util::Util::BytesToHexString(raw + 0, 4);

    String str = "[{'data': '0x%data1%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());

    auto rfir = rfir::GetRfir(this->name);
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;
    return encode->result;
};

bool rfir::module::device::test::Sniffer::onCmd_set(neb::CJsonObject* pld) {
    std::string code;
    if (pld->Get("code", code)) {        
        Serial.println(code.c_str());
        return rfir::util::Util::StringToBytes(code, this->protocol.remote_state);
    }
    return false;
};

bool rfir::module::device::test::Sniffer::onCmd_get(neb::CJsonObject* pld) {
    auto code = "0x" + rfir::util::Util::BytesToHexString(this->protocol.remote_state, KSnifferLength);
    return pld->Add("code", code);
};

bool rfir::module::device::test::Sniffer::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count >= 1) {
        return setRaw(data->result[0].bytes);
    }
    return false;

};
                    


//同事振明电动车315钥匙
rfir::module::ttl::Config::Device* rfir::module::device::test::Sniffer::initTempCodec() {
   this->name = "temp";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 30;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = false;

    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 30;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 24;
    dp[0].headermark = 0;
    dp[0].headerspace = 0;
    dp[0].onemark = 350;
    dp[0].onespace = 1200;
    dp[0].zeromark = 1150;
    dp[0].zerospace = 450;
    dp[0].footermark = 330;
    dp[0].footerspace = 12600;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;   


}       


//okonoff 
rfir::module::ttl::Config::Device* rfir::module::device::test::Sniffer::initOkonoffCodec() {
   this->name = "okonoff";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 30;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = true;

    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 30;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 112;
    dp[0].headermark = 3500;
    dp[0].headerspace = 1700;
    dp[0].onemark = 560;
    dp[0].onespace = 1150;
    dp[0].zeromark = 560;
    dp[0].zerospace = 340;
    dp[0].footermark = 560;
    dp[0].footerspace = 10000;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;   


} 
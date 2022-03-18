#include "sansui.h"


rfir::module::ttl::Config::Device* rfir::module::device::speaker::SANSUI::init() {
   this->name = "sansui";
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

    dp[0].tolerance = 20;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 32;
    dp[0].headermark = 8950;
    dp[0].headerspace = 4450;
    dp[0].onemark = 560;
    dp[0].onespace = 1700;
    dp[0].zeromark = 560;
    dp[0].zerospace = 560;
    dp[0].footermark = 560;
    dp[0].footerspace = 39000;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;    
}

bool rfir::module::device::speaker::SANSUI::setRaw(uint8_t* raw) {
    memcpy(protocol.remote_state, raw, KSansuiStateLength);
    this->onSetRaw();
    return true;
};

uint8_t* rfir::module::device::speaker::SANSUI::getRaw(int& count) {
    count = KSansuiStateLength;
    return this->protocol.remote_state;

};
           
uint16_t* rfir::module::device::speaker::SANSUI::getEncodeRaw(int& count) {
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

bool rfir::module::device::speaker::SANSUI::onCmd_set(neb::CJsonObject* pld) {
    std::string code;
    if (pld->Get("code", code)) {        
        DEBUGER.println(code.c_str());
        return rfir::util::Util::StringToBytes(code, this->protocol.remote_state);
    }
    return false;
};

bool rfir::module::device::speaker::SANSUI::onCmd_get(neb::CJsonObject* pld) {
    auto code = "0x" + rfir::util::Util::BytesToHexString(this->protocol.remote_state, KSansuiStateLength);
    return pld->Add("code", code);
};

bool rfir::module::device::speaker::SANSUI::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count >= 1) {
        return setRaw(data->result[0].bytes);
    }
    return false;

};
                    
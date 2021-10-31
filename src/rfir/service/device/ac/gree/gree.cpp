#include "gree.h"

#ifdef RFIR_DEVICE_GREE

IRGreeAC* rfir::service::device::ac::Gree::Device = new IRGreeAC(0);


rfir::module::ttl::Config::Device* rfir::service::device::ac::Gree::Init(std::string name) {
    auto ds = &rfir::RFIR::Config->devices;
    //创建设备
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 50;
    sp->minDelta = 150;
    sp->maxDelta = 65535;

    //解码参数
    d->packet.decode.create(3);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 30;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 32;
    dp[0].headermark = 9000;
    dp[0].headerspace = 4500;
    dp[0].onemark = 620;
    dp[0].onespace = 1600;
    dp[0].zeromark = 620;
    dp[0].zerospace = 540;
    dp[0].footermark = 0;
    dp[0].footerspace = 0;
    dp[0].lastspace = 0;

    dp[1] = dp[0];
    dp[1].use_bits = true;
    dp[1].nbits = 3;
    dp[1].headermark = 0;
    dp[1].headerspace = 0;
    dp[1].footermark = 620;
    dp[1].footerspace = 19000;

    dp[2] = dp[0];
    dp[2].nbits = 32;
    dp[2].headermark = 0;
    dp[2].headerspace = 0;
    dp[2].footermark = 620;
    dp[2].footerspace = 19000;    

    //编码参数
    d->packet.encode.clone(&d->packet.decode);
    return d;
}


bool rfir::service::device::ac::Gree::OnCmd(const char* cmd) {
    return true;

}

bool rfir::service::device::ac::Gree::OnCmd(neb::CJsonObject* cmd) {
    return true;
}




uint16_t* rfir::service::device::ac::Gree::getEncodeRaw(uint16_t& count) {
    auto raw = Device->getRaw();
    auto str1 = rfir::util::Util::BytesToString(raw, 4);
    uint8_t bits[1] = {0b010};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToString(raw + 3, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());
    return 0;


    auto rfir = rfir::GetRfir("gree");
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;
    return encode->result;
}

#endif
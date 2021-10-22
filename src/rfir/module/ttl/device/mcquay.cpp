#include "mcquay.h"

rfir::module::ttl::Config::Device* rfir::module::ttl::device::Mcquay::Init(rfir::module::ttl::Config::Devices* ds, std::string name) {
    //创建设备
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
    return d;
}

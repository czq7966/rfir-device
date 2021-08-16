#include "gree.h"

rfir::module::ttl::Config::Device* rfir::module::ttl::device::Gree::Init(rfir::module::ttl::Config::Devices* ds, std::string name) {
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

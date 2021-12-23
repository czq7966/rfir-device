#include "cl-djroom-light-mswitch2.h"


void rfir::module::device::mswitch::CL_DJROOM_Light_MSwitch2::start(void * p) {
    auto sw1 = new Switch();
    sw1->params.name = RFIR_MSWITCH_1_NAME;
    sw1->params.rfirToggle = RFIR_MSWITCH_1_CODE_TOGGLE;
    sw1->params.enabled = true;    
    sw1->params.pinIn = RFIR_MSWITCH_1_PIN_IN;
    sw1->params.pinOut = RFIR_MSWITCH_1_PIN_OUT;
    sw1->params.default_v = false;

    auto sw2 = new Switch();
    sw2->params.name = RFIR_MSWITCH_2_NAME;
    sw2->params.rfirToggle = RFIR_MSWITCH_2_CODE_TOGGLE;
    sw2->params.enabled = true;
    sw2->params.pinIn = RFIR_MSWITCH_2_PIN_IN;
    sw2->params.pinOut = RFIR_MSWITCH_2_PIN_OUT;    
    sw2->params.default_v = false;
    
    addSwitch(sw1);
    addSwitch(sw2);

    MSwitch::start(p);
};

rfir::module::ttl::Config::Device* rfir::module::device::mswitch::CL_DJROOM_Light_MSwitch2::init() {
   this->name = "mswitch";
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
    sp->pin = RFIR_MSWITCH_SNIFF_PIN;

// #define RFIF_LIGE 
#ifdef RFIF_LIGE 
    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 35;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 24;
    dp[0].headermark = 0;
    dp[0].headerspace = 0;
    dp[0].onemark = 280;
    dp[0].onespace = 800;
    dp[0].zeromark = 800;
    dp[0].zerospace = 280;
    dp[0].footermark = 280;
    dp[0].footerspace = 8500;
    dp[0].lastspace = 0;
#else
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
#endif

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    


    
    return d;    
}


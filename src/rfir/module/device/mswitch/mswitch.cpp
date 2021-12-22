#include "mswitch.h"


rfir::module::device::mswitch::MSwitch::~MSwitch() {
    destroySwitchs();
}

void rfir::module::device::mswitch::MSwitch::start(void * p) {
    Device::start(p);


};

void rfir::module::device::mswitch::MSwitch::loop() {
    Device::loop();    
    doSwitchChange();
}; 


bool  rfir::module::device::mswitch::MSwitch::addSwitch(Switch* switch_p) {
    auto n = std::string(switch_p->params.name.c_str());
    switchs[n] = switch_p;
    return true;
};

bool  rfir::module::device::mswitch::MSwitch::removeSwitch(Switch* switch_p) {
    auto n = std::string(switch_p->params.name.c_str());
    auto it = switchs.find(n);
    if (it != switchs.end())
        switchs.erase(it);
    return true;
};

void  rfir::module::device::mswitch::MSwitch::destroySwitchs() {
    auto it = switchs.begin();
    while (it != switchs.end()) {
        delete it->second;
        it++;
    }    
    switchs.clear();    
};


rfir::module::ttl::Config::Device* rfir::module::device::mswitch::MSwitch::init() {
   this->name = "mswitch";
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


bool rfir::module::device::mswitch::MSwitch::onCmd_set(neb::CJsonObject* pld) {
    auto it = switchs.begin();
    while (it != switchs.end()) {
        it->second->onCmd_set(pld);
        it++;
    }
    return true;
};

bool rfir::module::device::mswitch::MSwitch::onCmd_get(neb::CJsonObject* pld) {
    auto it = switchs.begin();
    while (it != switchs.end()) {
        it->second->onCmd_get(pld);
        it++;
    }
    return true;
};

bool rfir::module::device::mswitch::MSwitch::doSwitchChange() {
    if (rfir::module::device::mswitch::Switch::ChangedSwitch) {
        auto sw = (rfir::module::device::mswitch::Switch*)rfir::module::device::mswitch::Switch::ChangedSwitch;
        auto reason = "State Change: " + sw->params.name; 
        emitChange(reason.c_str());        
        rfir::module::device::mswitch::Switch::ChangedSwitch = 0;
    }

    return false;
}

bool rfir::module::device::mswitch::MSwitch::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {

    return false;

};
                    
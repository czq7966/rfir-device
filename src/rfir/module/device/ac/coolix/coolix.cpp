#include "coolix.h"



rfir::module::ttl::Config::Device* rfir::module::device::ac::Coolix::init() {
   this->name = "Coolix";
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

    dp[0].tolerance = 30;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = true;
    dp[0].step = 2;
    dp[0].use_bits = false;

    dp[0].nbits = CoolixAC::KCoolixStateLength * 8;
    dp[0].headermark = CoolixAC::KCoolixEncodeHeaderMark;
    dp[0].headerspace = CoolixAC::KCoolixEncodeHeaderSpace;
    dp[0].onemark = CoolixAC::KCoolixEncodeOneMark;
    dp[0].onespace = CoolixAC::KCoolixEncodeOneSpace;
    dp[0].zeromark = CoolixAC::KCoolixEncodeZeroMark;
    dp[0].zerospace = CoolixAC::KCoolixEncodeZeroSpace;
    dp[0].footermark = CoolixAC::KCoolixEncodeFooterMark;
    dp[0].footerspace = CoolixAC::KCoolixEncodeFooterSpace;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 1;    
    d->packet.send.params.modulation = true;    
    
    return d;   
}



rfir::module::device::ac::Coolix::Coolix() {
    this->ac = new CoolixAC(); 
}

rfir::module::device::ac::Coolix::~Coolix() {
    delete this->ac;
    this->ac = 0;
}


void rfir::module::device::ac::Coolix::start(void * p) {
    Device::start(p);
    this->ac->setRaw(this->ac->protocol.remote_state);
}


void rfir::module::device::ac::Coolix::loop() {
    Device::loop();
}

bool rfir::module::device::ac::Coolix::setRaw(uint8_t* raw) {
    this->ac->setRaw(raw);
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Coolix::getRaw(int& count) {
    count = CoolixAC::KCoolixStateLength;
    return this->ac->getRaw();
}

uint16_t* rfir::module::device::ac::Coolix::getEncodeRaw(int& count) {
    count = CoolixAC::KCoolixEncodeRawLength;
    return ac->getEncodeRaw();
}

bool rfir::module::device::ac::Coolix::onCmd_set(neb::CJsonObject* pld) {
    if (!pld ) return 0;

    //Power
    auto power = ac->getPower();
    std::string powerStr;
    pld->Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //恢复关闭前的状态
    ac->setPower(true);

    //Mode
    auto mode = ac->getMode();
    std::string modeStr;
    pld->Get("mode", modeStr);
    mode =  modeStr == "cool" ? kCoolixCool : 
            modeStr == "auto" ? kCoolixAuto :
            modeStr == "dry" ? kCoolixDry :
            modeStr == "fan" ? kCoolixFan :
            modeStr == "heat" ? kCoolixHeat : mode;

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? kCoolixFanAuto : 
            fanStr == "low" ? kCoolixFanMin : 
            fanStr == "medium" ? kCoolixFanMed : 
            fanStr == "high" ? kCoolixFanMax : fan;


    //Temp
    uint32 temp = ac->getTemp();
    pld->Get("temperature", temp);


    ac->setMode(mode);
    ac->setFan(fan);
    ac->setTemp(temp);

    ac->setPower(power);
    
    this->setRaw(ac->getRaw());

    return true;
}


bool rfir::module::device::ac::Coolix::onCmd_get(neb::CJsonObject* pld) {
    auto power = ac->getPower();
    ac->setPower(true);
    auto mode = ac->getMode();
    std::string modeStr =  getModeStr(mode);
    auto fan = ac->getFan();
    std::string fanStr =  getFanStr(fan);
    auto temp = ac->getTemp();
    ac->setPower(power);

    std::string powerStr = ac->getPower() ? "on" : "off";

    pld->Add("power", powerStr);
    pld->Add("mode", modeStr);
    pld->Add("fanSpeed", fanStr);
    pld->Add("temperature", temp);
    pld->Add("extra", ac->toBitString() + "," + ac->toHexString());

    return true;
}

bool rfir::module::device::ac::Coolix::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count >= 1) {
        int size = 0;
        auto raw = getRaw(size);
        memcpy(raw, data->result[0].bytes, size);;
        return setRaw(raw);
    }
    return false;
}

std::string rfir::module::device::ac::Coolix::getModeStr(uint8_t mode) {
    std::string modeStr = String(mode).c_str();
    switch (mode)
    {
        case kCoolixCool:
            modeStr = "cool";
            break;
        case kCoolixDry:
            modeStr = "dry";
            break;
        case kCoolixAuto:
            modeStr = "auto";
            break;
        case kCoolixHeat:
            modeStr = "heat";
            break;
        case kCoolixFan:
            modeStr = "fan";
            break;                                            
        default:
            break;
    }

    return modeStr;
}

std::string rfir::module::device::ac::Coolix::getFanStr(uint8_t fan) {
    std::string str = String(fan).c_str();
    switch (fan)
    {
        case kCoolixFanMin:
            str = "low";
            break;
        case kCoolixFanMed:
            str = "medium";
            break;
        case kCoolixFanMax:
            str = "high";
            break;
        case kCoolixFanAuto:
        case kCoolixFanAuto0:
        case kCoolixFanZoneFollow:
        case kCoolixFanFixed:
            str = "auto";
            break;
                                      
        default:
            break;
    }

    return str;
}

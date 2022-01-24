#include "okonoff.h"
#include "okonoff-ac.h"



rfir::module::ttl::Config::Device* rfir::module::device::ac::Okonoff::init() {
   this->name = "okonoff";
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
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;

    dp[0].nbits = OkonoffAC::KOkonoffStateLength * 8;
    dp[0].headermark = OkonoffAC::KOkonoffEncodeHeaderMark;
    dp[0].headerspace = OkonoffAC::KOkonoffEncodeHeaderSpace;
    dp[0].onemark = OkonoffAC::KOkonoffEncodeOneMark;
    dp[0].onespace = OkonoffAC::KOkonoffEncodeOneSpace;
    dp[0].zeromark = OkonoffAC::KOkonoffEncodeZeroMark;
    dp[0].zerospace = OkonoffAC::KOkonoffEncodeZeroSpace;
    dp[0].footermark = OkonoffAC::KOkonoffEncodeFooterMark;
    dp[0].footerspace = OkonoffAC::KOkonoffEncodeFooterSpace;
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
    this->ac = new OkonoffAC(); 
}

rfir::module::device::ac::Okonoff::~Okonoff() {
    delete this->ac;
    this->ac = 0;
}


void rfir::module::device::ac::Okonoff::start(void * p) {
    Device::start(p);

}


void rfir::module::device::ac::Okonoff::loop() {
    Device::loop();
}

bool rfir::module::device::ac::Okonoff::setRaw(uint8_t* raw) {
    this->ac->setRaw(raw);
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Okonoff::getRaw(int& count) {
    count = OkonoffAC::KOkonoffStateLength;
    return this->ac->getRaw();
}

uint16_t* rfir::module::device::ac::Okonoff::getEncodeRaw(int& count) {
    count = OkonoffAC::KOkonoffEncodeRawLength;
    return ac->getEncodeRaw();
}

bool rfir::module::device::ac::Okonoff::onCmd_set(neb::CJsonObject* pld) {
    if (!pld ) return 0;

    //Power
    auto power = ac->getPower();
    std::string powerStr;
    pld->Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac->getMode();
    std::string modeStr;
    pld->Get("mode", modeStr);
    mode =  modeStr == "cool" ? OkonoffAC::KOkonoffModeCool : 
            modeStr == "heat" ? OkonoffAC::KOkonoffModeHeat : mode;

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? OkonoffAC::KOkonoffFanAuto : 
            fanStr == "low" ? OkonoffAC::KOkonoffFanLow : 
            fanStr == "medium" ? OkonoffAC::KOkonoffFanMed : 
            fanStr == "high" ? OkonoffAC::KOkonoffFanHigh : fan;


    //Temp
    uint32 temp = ac->getTemp();
    pld->Get("temperature", temp);



    ac->setPower(power);
    ac->setMode(mode);
    ac->setFan(fan);
    ac->setTemp(temp);

    
    this->setRaw(ac->getRaw());

    return true;
}


bool rfir::module::device::ac::Okonoff::onCmd_get(neb::CJsonObject* pld) {
    ac->fixup();
    auto mode = ac->getMode();
    std::string modeStr =  getModeStr(mode);
    auto fan = ac->getFan();
    std::string fanStr =  getFanStr(fan);

    std::string powerStr = ac->getPower() ? "on" : "off";

    pld->Add("power", powerStr);
    pld->Add("mode", modeStr);
    pld->Add("fanSpeed", fanStr);
    pld->Add("temperature", ac->getTemp());
    pld->Add("extra", ac->toBitString() + "," + ac->toHexString());

    return true;
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

std::string rfir::module::device::ac::Okonoff::getModeStr(uint8_t mode) {
    std::string modeStr =   mode == OkonoffAC::KOkonoffModeCool ? "cool" :
                            mode == OkonoffAC::KOkonoffModeHeat ? "heat" : String(mode).c_str();
    return modeStr;
}

std::string rfir::module::device::ac::Okonoff::getFanStr(uint8_t fan) {
    std::string fanStr =    fan == OkonoffAC::KOkonoffFanAuto ? "auto" :
                            fan == OkonoffAC::KOkonoffFanLow ? "low" :
                            fan == OkonoffAC::KOkonoffFanMed ? "medium" :
                            fan == OkonoffAC::KOkonoffFanHigh ? "high" : "unknown";
    return fanStr;
}

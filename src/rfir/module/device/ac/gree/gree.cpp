#include "gree.h"
#include "gree-ac.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::Gree::init() {
    this->name = "gree";
    auto ds = &rfir::RFIR::Config->devices;
    //创建设备
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 50;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = true;

    //发码参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;        

    //解码参数
    d->packet.decode.create(3);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 20;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 32;
    dp[0].headermark = GreeAC::KGreeHdrMark;
    dp[0].headerspace = GreeAC::KGreeHdrSpace;
    dp[0].onemark = GreeAC::KGreeBitMark;
    dp[0].onespace = GreeAC::KGreeOneSpace;
    dp[0].zeromark = GreeAC::KGreeBitMark;
    dp[0].zerospace = GreeAC::KGreeZeroSpace;
    dp[0].footermark = 0;
    dp[0].footerspace = 0;
    dp[0].lastspace = 0;

    dp[1] = dp[0];
    dp[1].use_bits = true;
    dp[1].nbits = 3;
    dp[1].headermark = 0;
    dp[1].headerspace = 0;
    dp[1].footermark = GreeAC::KGreeBitMark;
    dp[1].footerspace = GreeAC::KGreeMsgSpace;

    dp[2] = dp[0];
    dp[2].nbits = 32;
    dp[2].headermark = 0;
    dp[2].headerspace = 0;
    dp[2].footermark = GreeAC::KGreeBitMark;
    dp[2].footerspace = GreeAC::KGreeMsgSpace;    

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    
    return d;
}



rfir::module::device::ac::Gree::Gree() {

}

rfir::module::device::ac::Gree::~Gree() {

}


bool rfir::module::device::ac::Gree::setRaw(uint8_t* raw) {
    this->ac.ac->setRaw(raw);
    dump();

    // this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Gree::getRaw(int& count) {
    count = kGreeStateLength;
    return this->ac.ac->getRaw();
}

uint16_t* rfir::module::device::ac::Gree::getEncodeRaw(int& count) {
    return 0;
    // count = GreeAC::KGreeEncodeRawLength;
    // return this->ac.getEncodeRaw();
Serial.println("11111");    
    int length = 0;
    auto raw = getRaw(length);
    auto str1 = rfir::util::Util::BytesToString(raw, 4);
    uint8_t bits[1] = {0b010};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToString(raw + 3, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
Serial.println(str);
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());
Serial.println("22222");    
    auto rfir = rfir::GetRfir(this->name);
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;
Serial.println("33333");        
    return encode->result;    
//     return 0;
}

bool rfir::module::device::ac::Gree::onCmd_set(neb::CJsonObject* pld) {
    if (!pld) return 0;
    

    //Power
    auto power = ac.ac->getPower();
    std::string powerStr;
    pld->Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac.ac->getMode();
    std::string modeStr;
    pld->Get("mode", modeStr);
    mode =  modeStr == "auto" ? kGreeAuto : 
            modeStr == "cool" ? kGreeCool : 
            modeStr == "dry" ? kGreeDry : 
            modeStr == "fan" ? kGreeFan : 
            modeStr == "fan_only" ? kGreeFan : 
            modeStr == "heat" ? kGreeHeat : mode;

    //FanSpeed
    auto fan = ac.ac->getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? kGreeFanAuto : 
            fanStr == "low" ? kGreeFanMin : 
            fanStr == "medium" ? kGreeFanMed : 
            fanStr == "high" ? kGreeFanMax : fan;

    //Sleep
    auto sleep = ac.ac->getSleep();
    std::string sleepStr;
    pld->Get("sleep", sleepStr);
    sleep = sleepStr == "on" ? true : 
            sleepStr == "off" ? false : sleep;

    //Swing
    auto swing = ac.ac->getSwingVerticalAuto();
    std::string swingStr;
    pld->Get("swing", swingStr);
    swing = swingStr == "on" ? true : 
            swingStr == "off" ? false : swing;

    //Temp
    uint32 temp = ac.ac->getTemp();
    pld->Get("temperature", temp);


    ac.ac->setMode(mode);
    ac.ac->setFan(fan);
    ac.ac->setSleep(sleep);
    ac.ac->setSwingVertical(swing, kGreeSwingAuto);
    ac.ac->setTemp(temp);
    ac.ac->setPower(power);

    this->setRaw(ac.ac->getRaw());
    return true;
}


bool rfir::module::device::ac::Gree::onCmd_get(neb::CJsonObject* pld) {
    //Power
    pld->Add("power", ac.ac->getPower() ? "on" : "off");

    //Mode
    auto mode = ac.ac->getMode();
    std::string modeStr =   mode == kGreeAuto ? "auto" :
                            mode == kGreeCool ? "cool" : 
                            mode == kGreeDry ? "dry" : 
                            mode == kGreeFan ? "fan" : 
                            mode == kGreeFan ? "fan_only" : 
                            mode == kGreeHeat ? "heat" : std::string(String(mode).c_str());
    pld->Add("mode", modeStr);

    //FanSpeed
    auto fan = ac.ac->getFan();
    std::string fanStr =    fan == kGreeFanAuto ? "auto" : 
                            fan == kGreeFanMin ? "low" : 
                            fan == kGreeFanMed ? "medium" : 
                            fan == kGreeFanMax ? "high" : std::string(String(fan).c_str());
    pld->Add("fanSpeed", fanStr);

    //Sleep
    pld->Add("sleep", ac.ac->getSleep() ? "on" : "off");

    //Swing
    pld->Add("swing", ac.ac->getSwingVerticalAuto() ? "on" : "off");

    //Temp
    pld->Add("temperature", ac.ac->getTemp());
    pld->Add("extra", ac.toBitString() + "," + ac.toHexString());

    return true;
}

bool rfir::module::device::ac::Gree::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count == 3) {
        int count = 0;
        auto raw = getRaw(count);
        memcpy(raw, data->result[0].bytes, 4);
        memcpy(raw + 3, data->result[2].bytes, 4);
        return setRaw(raw);
    }
    return false;
}

void  rfir::module::device::ac::Gree::dump() {
    int length = 0;
    auto raw = getRaw(length);
    auto str1 = rfir::util::Util::BytesToHexString(raw, 4);
    uint8_t bits[1] = {0b010};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToHexString(raw + 3, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
    Serial.println(str);
}


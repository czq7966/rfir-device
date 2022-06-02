#include "gree.h"
#include "gree-ac.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::Gree::init() {
    this->name = "Gree";
      
    decoder->decodeParams.push_back({});
    decoder->decodeParams.push_back({});
    decoder->decodeParams.push_back({});
    
    auto dp0 = decoder->decodeParams[0];
    dp0.tolerance = 20;
    dp0.excess = 0;
    dp0.atleast = true;                              
    dp0.MSBfirst = false;
    dp0.step = 2;
    dp0.use_bits = false;
    
    dp0.nbits = 32;
    dp0.headermark = GreeAC::KGreeHdrMark;
    dp0.headerspace = GreeAC::KGreeHdrSpace;
    dp0.onemark = GreeAC::KGreeBitMark;
    dp0.onespace = GreeAC::KGreeOneSpace;
    dp0.zeromark = GreeAC::KGreeBitMark;
    dp0.zerospace = GreeAC::KGreeZeroSpace;
    dp0.footermark = 0;
    dp0.footerspace = 0;
    dp0.lastspace = 0;
    decoder->decodeParams[0] = dp0;

    auto dp1 = dp0;
    dp1.use_bits = true;
    dp1.nbits = 3;
    dp1.headermark = 0;
    dp1.headerspace = 0;
    dp1.footermark = GreeAC::KGreeBitMark;
    dp1.footerspace = GreeAC::KGreeMsgSpace;
    decoder->decodeParams[1] = dp1;

    auto dp2 = dp0;
    dp2.nbits = 32;
    dp2.headermark = 0;
    dp2.headerspace = 0;
    dp2.footermark = GreeAC::KGreeBitMark;
    dp2.footerspace = GreeAC::KGreeMsgSpace;  
    decoder->decodeParams[2] = dp2;

    encoder->encodeParams.push_back({});
    encoder->encodeParams.push_back({});
    encoder->encodeParams.push_back({});
    auto ep0 = encoder->encodeParams[0];
    ep0.clone(&dp0);
    encoder->encodeParams[0] = ep0;

    auto ep1 = encoder->encodeParams[1];
    ep1.clone(&dp1);
    encoder->encodeParams[1] = ep1;

    auto ep2 = encoder->encodeParams[2];
    ep2.clone(&dp2);
    encoder->encodeParams[2] = ep2;

    return 0;
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
    int length = 0;
    auto raw = getRaw(length);
    auto str1 = rfir::util::Util::BytesToString(raw, 4);
    uint8_t bits[1] = {0b010};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToString(raw + 4, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());
    auto rfir = rfir::GetRfir(this->name);
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;

    return encode->result;    
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
        memcpy(raw + 4, data->result[2].bytes, 4);
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
    auto str3 = rfir::util::Util::BytesToHexString(raw + 4, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
    DEBUGER.println(str);
}


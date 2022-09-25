#include "gree.h"
#include "gree-ac.h"


void rfir::module::device::ac::Gree::init() {
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
}


bool rfir::module::device::ac::Gree::setRaw(uint8_t* raw) {
    IRGreeAC ac(0);
    ac.setRaw(raw);

    this->ac.ac->setMode(ac.getMode());
    this->ac.ac->setFan(ac.getFan());
    // this->ac.ac->setSleep(ac.getSleep());
    // this->ac.ac->setSwingVertical(ac.getSwingVerticalAuto(), kGreeSwingAuto);
    this->ac.ac->setTemp(ac.getTemp());
    this->ac.ac->setPower(ac.getPower());

    return 1;
}

uint8_t* rfir::module::device::ac::Gree::getRaw(int& count) {
    count = kGreeStateLength;
    return this->ac.ac->getRaw();
}

bool rfir::module::device::ac::Gree::getEncodeRaw(std::list<uint16_t>& result) {   
    int count = 0;
    auto raw = getRaw(count);
    auto str1 = rfir::util::Util::BytesToString(raw, 4);
    uint8_t bits[1] = {0b010};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToString(raw + 4, 4);

    std::vector<std::string> data;
    data.push_back(str1);
    data.push_back(str2);
    data.push_back(str3);
    return encoder->encode(data, result);
}

int rfir::module::device::ac::Gree::onSvc_set(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    if (!pld) return 0;
    

    //Power
    auto power = ac.ac->getPower();
    std::string powerStr = pld->containsKey("power") ? (*pld)["power"].as<std::string>() : "";
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac.ac->getMode();
    std::string modeStr = pld->containsKey("mode") ? (*pld)["mode"].as<std::string>() : "";
    mode =  modeStr == "auto" ? kGreeAuto : 
            modeStr == "cool" ? kGreeCool : 
            modeStr == "dry" ? kGreeDry : 
            modeStr == "fan" ? kGreeFan : 
            modeStr == "fan_only" ? kGreeFan : 
            modeStr == "heat" ? kGreeHeat : mode;

    //FanSpeed
    auto fan = ac.ac->getFan();
    std::string fanStr = pld->containsKey("fanSpeed") ? (*pld)["fanSpeed"].as<std::string>() : "";
    fan =   fanStr == "auto" ? kGreeFanAuto : 
            fanStr == "low" ? kGreeFanMin : 
            fanStr == "medium" ? kGreeFanMed : 
            fanStr == "high" ? kGreeFanMax : fan;

    //Sleep
    auto sleep = ac.ac->getSleep();
    std::string sleepStr = pld->containsKey("sleep") ? (*pld)["sleep"].as<std::string>() : "";
    sleep = sleepStr == "on" ? true : 
            sleepStr == "off" ? false : sleep;

    //Swing
    auto swing = ac.ac->getSwingVerticalAuto();
    std::string swingStr = pld->containsKey("swing") ? (*pld)["swing"].as<std::string>() : "";
    swing = swingStr == "on" ? true : 
            swingStr == "off" ? false : swing;

    //Temp
    uint32 temp = pld->containsKey("temperature") ? (*pld)["temperature"] : ac.ac->getTemp();

    ac.ac->setMode(mode);
    ac.ac->setFan(fan);
    ac.ac->setSleep(sleep);
    ac.ac->setSwingVertical(swing, kGreeSwingAuto);
    ac.ac->setTemp(temp);
    ac.ac->setPower(power);

    saveConfig();
    sendRaw();
    return true;
}


int rfir::module::device::ac::Gree::onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) {
    //Power
    (*pld)["power"] = ac.ac->getPower() ? "on" : "off";

    //Mode
    auto mode = ac.ac->getMode();
    std::string modeStr =   mode == kGreeAuto ? "auto" :
                            mode == kGreeCool ? "cool" : 
                            mode == kGreeDry ? "dry" : 
                            mode == kGreeFan ? "fan" : 
                            mode == kGreeFan ? "fan_only" : 
                            mode == kGreeHeat ? "heat" : std::string(String(mode).c_str());
    (*pld)["mode"] = modeStr;

    //FanSpeed
    auto fan = ac.ac->getFan();
    std::string fanStr =    fan == kGreeFanAuto ? "auto" : 
                            fan == kGreeFanMin ? "low" : 
                            fan == kGreeFanMed ? "medium" : 
                            fan == kGreeFanMax ? "high" : std::string(String(fan).c_str());
    (*pld)["fanSpeed"] = fanStr;

    //Sleep
    (*pld)["sleep"] = ac.ac->getSleep() ? "on" : "off";

    //Swing
    (*pld)["swing"] = ac.ac->getSwingVerticalAuto() ? "on" : "off";

    //Temp
    (*pld)["temperature"] = ac.ac->getTemp();
    (*pld)["extra"] = ac.toBitString() + "," + ac.toHexString();

    return true;
}

int rfir::module::device::ac::Gree::onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& data) {
    static unsigned long last_decoded_time = 0;
    bool result = false;
    if (data.size() == 3 && (millis() - last_decoded_time > 500)) {
        int count = 0;
        auto raw = getRaw(count);
        uint8_t bytes[count];
        
        memcpy(bytes, data[0].bytes, 4);
        memcpy(bytes + 4, data[2].bytes, 4);
        result = setRaw(bytes);
        delaySaveConfig();  
    }
    if (result) 
        last_decoded_time = millis();
    return result;
}



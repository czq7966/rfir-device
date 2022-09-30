#include "midea.h"
#include "midea-ac.h"

void rfir::module::device::ac::Midea::init() {
    this->name = "Midea";
      
    decoder->decodeParams.push_back({});    
    auto dp0 = decoder->decodeParams[0];
    dp0.tolerance = 20;
    dp0.excess = 0;
    dp0.atleast = true;                              
    dp0.MSBfirst = true;
    dp0.step = 2;
    dp0.use_bits = false;
    
    dp0.nbits = MideaAC::KBits;
    dp0.headermark = MideaAC::KHdrMark;
    dp0.headerspace = MideaAC::KHdrSpace;
    dp0.onemark = MideaAC::KOneMark;
    dp0.onespace = MideaAC::KOneSpace;
    dp0.zeromark = MideaAC::KZeroMark;
    dp0.zerospace = MideaAC::KZeroSpace;
    dp0.footermark = MideaAC::KFtrMark;
    dp0.footerspace = MideaAC::KFtrSpace;
    dp0.lastspace = 0;
    decoder->decodeParams[0] = dp0;

    encoder->encodeParams.push_back({});
    auto ep0 = encoder->encodeParams[0];
    ep0.clone(&dp0);
    encoder->encodeParams[0] = ep0;
}


bool rfir::module::device::ac::Midea::setRaw(uint8_t* raw) {
    ac.setRaw(raw);    
    return 1;
}

uint8_t* rfir::module::device::ac::Midea::getRaw(int& count) {
    return ac.getRaw(count);
}

bool rfir::module::device::ac::Midea::getEncodeRaw(std::list<uint16_t>& result) {   
    int count = 0;
    auto raw = getRaw(count);

    if (!ac.getPower())
        raw = (uint8_t*)&ac.protocol_close.remote_state;

    auto str1 = rfir::util::Util::BytesToString(raw, count);

    std::vector<std::string> data;
    data.push_back(str1);
    return encoder->encode(data, result);
}

int rfir::module::device::ac::Midea::onSvc_set(JsonObject* _pld, cmds::cmd::CmdBase* cmd) {
     if (!_pld) return 0;
    JsonObject pld = *_pld; 
    

    //Power
    auto power = ac.getPower();
    std::string powerStr = pld["power"];
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : power;

    //Mode
    auto mode = ac.getMode();
    std::string modeStr = pld["mode"];
    mode =  modeStr == "auto" ? ac.P_Mode_Auto : 
            modeStr == "cool" ? ac.P_Mode_Cool : 
            modeStr == "dry" ? ac.P_Mode_Dry : 
            modeStr == "fan" ? ac.P_Mode_Fan : 
            modeStr == "fan_only" ? ac.P_Mode_Fan : 
            modeStr == "heat" ? ac.P_Mode_Heat : mode;

    //FanSpeed
    auto fan = ac.getFan();
    std::string fanStr = pld["fanSpeed"];
    fan =   fanStr == "auto" ? ac.P_Fan_Auto : 
            fanStr == "low" ? ac.P_Fan_Low : 
            fanStr == "medium" ? ac.P_Fan_Med : 
            fanStr == "high" ? ac.P_Fan_High : fan;




    //Temp
    uint32 temp = pld.containsKey("temperature") ? pld["temperature"].as<uint32>() : ac.getTemp();


    ac.setMode(mode);
    ac.setFan(fan);
    ac.setTemp(temp);
    ac.setPower(power);

    saveConfig();
    sendRaw();
    return true;
}


int rfir::module::device::ac::Midea::onSvc_get(JsonObject* _pld, cmds::cmd::CmdBase* cmd) {
    JsonObject pld = *_pld;
    //Power
    pld["power"] = ac.getPower() ? "on" : "off";

    //Mode
    auto mode = ac.getMode();
    std::string modeStr =   mode == ac.P_Mode_Auto ? "auto" :
                            mode == ac.P_Mode_Cool  ? "cool" : 
                            mode == ac.P_Mode_Dry  ? "dry" : 
                            mode == ac.P_Mode_Fan  ? "fan" : 
                            mode == ac.P_Mode_Fan  ? "fan_only" : 
                            mode == ac.P_Mode_Heat  ? "heat" : std::string(String(mode).c_str());
    pld["mode"] = modeStr;

    //FanSpeed
    auto fan = ac.getFan();
    std::string fanStr =    fan == ac.P_Fan_Auto ? "auto" : 
                            fan == ac.P_Fan_Low ? "low" : 
                            fan == ac.P_Fan_Med ? "medium" : 
                            fan == ac.P_Fan_High ? "high" : std::string(String(fan).c_str());
    pld["fanSpeed"] = fanStr;

    //Temp
    pld["temperature"] = ac.getTemp();
    pld["extra"] = ac.toBitString() + "," + ac.toHexString();

    return true;
}

int rfir::module::device::ac::Midea::onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& data) {
    DEBUGER.println("rfir::module::device::ac::Midea::onSvc_decoded");    
    
    auto result = setRaw(data[0].bytes);
    delaySaveConfig();
    return result;
}



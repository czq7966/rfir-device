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

bool rfir::module::device::ac::Midea::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
     if (!pld) return 0;
    

    //Power
    auto power = ac.getPower();
    std::string powerStr;
    pld->Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac.getMode();
    std::string modeStr;
    pld->Get("mode", modeStr);
    mode =  modeStr == "auto" ? ac.P_Mode_Auto : 
            modeStr == "cool" ? ac.P_Mode_Cool : 
            modeStr == "dry" ? ac.P_Mode_Dry : 
            modeStr == "fan" ? ac.P_Mode_Fan : 
            modeStr == "fan_only" ? ac.P_Mode_Fan : 
            modeStr == "heat" ? ac.P_Mode_Heat : mode;

    //FanSpeed
    auto fan = ac.getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? ac.P_Fan_Auto : 
            fanStr == "low" ? ac.P_Fan_Low : 
            fanStr == "medium" ? ac.P_Fan_Med : 
            fanStr == "high" ? ac.P_Fan_High : fan;




    //Temp
    uint32 temp = ac.getTemp();
    pld->Get("temperature", temp);


    ac.setMode(mode);
    ac.setFan(fan);
    ac.setTemp(temp);
    ac.setPower(power);

    saveConfig();
    sendRaw();
    return true;
}


bool rfir::module::device::ac::Midea::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    //Power
    pld->Add("power", ac.getPower() ? "on" : "off");

    //Mode
    auto mode = ac.getMode();
    std::string modeStr =   mode == ac.P_Mode_Auto ? "auto" :
                            mode == ac.P_Mode_Cool  ? "cool" : 
                            mode == ac.P_Mode_Dry  ? "dry" : 
                            mode == ac.P_Mode_Fan  ? "fan" : 
                            mode == ac.P_Mode_Fan  ? "fan_only" : 
                            mode == ac.P_Mode_Heat  ? "heat" : std::string(String(mode).c_str());
    pld->Add("mode", modeStr);

    //FanSpeed
    auto fan = ac.getFan();
    std::string fanStr =    fan == ac.P_Fan_Auto ? "auto" : 
                            fan == ac.P_Fan_Low ? "low" : 
                            fan == ac.P_Fan_Med ? "medium" : 
                            fan == ac.P_Fan_High ? "high" : std::string(String(fan).c_str());
    pld->Add("fanSpeed", fanStr);

    //Temp
    pld->Add("temperature", ac.getTemp());
    pld->Add("extra", ac.toBitString() + "," + ac.toHexString());

    return true;
}

bool rfir::module::device::ac::Midea::onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& data) {
    DEBUGER.println("rfir::module::device::ac::Midea::onSvc_decoded");    
    
    auto result = setRaw(data[0].bytes);
    delaySaveConfig();
    return result;
}





// rfir::module::ttl::Config::Device* rfir::module::device::ac::Midea::init() {
//     this->name =  this->name != "" ? this->name : "Midea";
//     auto ds = &rfir::RFIR::Config->devices;
//     //创建设备
//     auto d = ds->newDevice(name);
//     if (!d) return 0;

//     //采码参数
//     auto sp = &d->packet.sniff.params;
//     sp->minCount = 50;
//     sp->minDelta = 150;
//     sp->maxDelta = 65535;
//     sp->inverted = true;
//     sp->mode = INPUT;

//     //发码参数
//     d->packet.send.params.inverted = false;
//     d->packet.send.params.repeat = 1;    
//     d->packet.send.params.modulation = true;        

//     //解码参数
//     d->packet.decode.create(1);    
//     auto dp = d->packet.decode.params;

//     dp[0].tolerance = 20;
//     dp[0].excess = 0;
//     dp[0].atleast = true;                              
//     dp[0].MSBfirst = true;
//     dp[0].step = 2;
//     dp[0].use_bits = false;
    
//     dp[0].nbits = MideaAC::KBits;
//     dp[0].headermark = MideaAC::KHdrMark;
//     dp[0].headerspace = MideaAC::KHdrSpace;
//     dp[0].onemark = MideaAC::KOneMark;
//     dp[0].onespace = MideaAC::KOneSpace;
//     dp[0].zeromark = MideaAC::KZeroMark;
//     dp[0].zerospace = MideaAC::KZeroSpace;
//     dp[0].footermark = MideaAC::KFtrMark;
//     dp[0].footerspace = MideaAC::KFtrSpace;
//     dp[0].lastspace = 0;


//     //编码参数
//     d->packet.encode.clone(&d->packet.decode);

    
//     return d;
// }


// bool rfir::module::device::ac::Midea::setRaw(uint8_t* raw) {
//     ac.setRaw(raw);    
//     this->onSetRaw();
//     return 1;
// }

// uint8_t* rfir::module::device::ac::Midea::getRaw(int& count) {    
//     return ac.getRaw(count);
// }

// uint16_t* rfir::module::device::ac::Midea::getEncodeRaw(int& count) {
//     return ac.getEncodeRaw(count);
//     // uint8_t* raw;
//     // if (ac.getPower())
//     //     raw = (uint8_t*)&ac.protocol.remote_state;
//     // else
//     //     raw = (uint8_t*)&ac.protocol_close.remote_state;
    

//     // auto strRaw = rfir::util::Util::BytesToString(raw, ac.KBytesLength);

//     // String str = "[{'data': '%data%'}]";
//     // str.replace("'", "\"");
//     // str.replace("%data%", strRaw.c_str());
    
//     // neb::CJsonObject blocks;
//     // blocks.Parse(str.c_str());
//     // auto rfir = rfir::GetRfir(this->name);
//     // rfir->encoder->encode(&blocks);
//     // auto encode = rfir->encoder->getEncodeResult();    
//     // count = encode->count;

//     // return encode->result;    
// }

// bool rfir::module::device::ac::Midea::onCmd_set(neb::CJsonObject* pld) {
//     if (!pld) return 0;
    

//     //Power
//     auto power = ac.getPower();
//     std::string powerStr;
//     pld->Get("power", powerStr);
//     power = powerStr == "on" ? true : 
//             powerStr == "off" ? false : true;

//     //Mode
//     auto mode = ac.getMode();
//     std::string modeStr;
//     pld->Get("mode", modeStr);
//     mode =  modeStr == "auto" ? ac.P_Mode_Auto : 
//             modeStr == "cool" ? ac.P_Mode_Cool : 
//             modeStr == "dry" ? ac.P_Mode_Dry : 
//             modeStr == "fan" ? ac.P_Mode_Fan : 
//             modeStr == "fan_only" ? ac.P_Mode_Fan : 
//             modeStr == "heat" ? ac.P_Mode_Heat : mode;

//     //FanSpeed
//     auto fan = ac.getFan();
//     std::string fanStr;
//     pld->Get("fanSpeed", fanStr);
//     fan =   fanStr == "auto" ? ac.P_Fan_Auto : 
//             fanStr == "low" ? ac.P_Fan_Low : 
//             fanStr == "medium" ? ac.P_Fan_Med : 
//             fanStr == "high" ? ac.P_Fan_High : fan;




//     //Temp
//     uint32 temp = ac.getTemp();
//     pld->Get("temperature", temp);


//     ac.setMode(mode);
//     ac.setFan(fan);
//     ac.setTemp(temp);
//     ac.setPower(power);

//     int count;
//     auto raw = ac.getRaw(count);
//     this->setRaw(raw);
//     return true;
// }


// bool rfir::module::device::ac::Midea::onCmd_get(neb::CJsonObject* pld) {
//     //Power
//     pld->Add("power", ac.getPower() ? "on" : "off");

//     //Mode
//     auto mode = ac.getMode();
//     std::string modeStr =   mode == ac.P_Mode_Auto ? "auto" :
//                             mode == ac.P_Mode_Cool  ? "cool" : 
//                             mode == ac.P_Mode_Dry  ? "dry" : 
//                             mode == ac.P_Mode_Fan  ? "fan" : 
//                             mode == ac.P_Mode_Fan  ? "fan_only" : 
//                             mode == ac.P_Mode_Heat  ? "heat" : std::string(String(mode).c_str());
//     pld->Add("mode", modeStr);

//     //FanSpeed
//     auto fan = ac.getFan();
//     std::string fanStr =    fan == ac.P_Fan_Auto ? "auto" : 
//                             fan == ac.P_Fan_Low ? "low" : 
//                             fan == ac.P_Fan_Med ? "medium" : 
//                             fan == ac.P_Fan_High ? "high" : std::string(String(fan).c_str());
//     pld->Add("fanSpeed", fanStr);

//     //Temp
//     pld->Add("temperature", ac.getTemp());
//     pld->Add("extra", ac.toBitString() + "," + ac.toHexString());

//     return true;
// }

// bool rfir::module::device::ac::Midea::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
//     DEBUGER.println("rfir::module::device::ac::Midea::onCmd_decoded");    
//     return setRaw(data->result[0].bytes);
// }

// void  rfir::module::device::ac::Midea::dump() {

//     int length = 0;
//     auto raw = getRaw(length);
//     auto strRaw = rfir::util::Util::BytesToHexString(raw, length);

//     String str = "[{'data': '%data%'}]";
//     str.replace("'", "\"");
//     str.replace("%data%", strRaw.c_str());
//     DEBUGER.println(str);



//     neb::CJsonObject pld;
//     onCmd_get(&pld);

//     DEBUGER.println(pld.ToString().c_str());

// }


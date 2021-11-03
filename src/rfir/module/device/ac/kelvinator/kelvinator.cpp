#include "kelvinator.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::Kelvinator::init() {
    this->name = "kelvinator";
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
    d->packet.decode.create(6);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 20;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 4 * 8;
    dp[0].headermark = kKelvinatorHdrMark;
    dp[0].headerspace = kKelvinatorHdrSpace;
    dp[0].onemark = kKelvinatorBitMark;
    dp[0].onespace = kKelvinatorOneSpace;
    dp[0].zeromark = kKelvinatorBitMark;
    dp[0].zerospace = kKelvinatorZeroSpace;
    dp[0].footermark = 0;
    dp[0].footerspace = 0;
    dp[0].lastspace = 0;

    dp[1] = dp[0];
    dp[1].nbits = kKelvinatorCmdFooterBits;
    dp[1].use_bits = true;    
    dp[1].headermark = 0;
    dp[1].headerspace = 0;
    dp[1].footermark = kKelvinatorBitMark;
    dp[1].footerspace = kKelvinatorGapSpace;

    dp[2] = dp[0];
    dp[2].headermark = 0;
    dp[2].headerspace = 0;
    dp[2].footermark = kKelvinatorBitMark;
    dp[2].footerspace = kKelvinatorGapSpace * 2;    

    dp[3] = dp[0];
    dp[4] = dp[1];
    dp[5] = dp[2];


    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    
    return d;
}



rfir::module::device::ac::Kelvinator::Kelvinator() {
 
}

rfir::module::device::ac::Kelvinator::~Kelvinator() {
    if (ac) {
        delete this->ac;
        this->ac = 0;
    }
}

void rfir::module::device::ac::Kelvinator::start(void *) {
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->getDevice(this->name);
    Serial.println("aaaaaaaaaaaaaa");
    Serial.println(d->packet.send.params.pin);
    this->ac = new IRKelvinatorAC(d->packet.send.params.pin); 
}


bool rfir::module::device::ac::Kelvinator::setRaw(uint8_t* raw) {
    this->ac->setRaw(raw);
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Kelvinator::getRaw(int& count) {
    count = kKelvinatorStateLength;
    return this->ac->getRaw();
}

uint16_t* rfir::module::device::ac::Kelvinator::getEncodeRaw(int& count) {
    int length = 0;
    auto raw = getRaw(length);
    auto str1 = rfir::util::Util::BytesToString(raw + 0, 4);
    uint8_t bits[1] = {kKelvinatorCmdFooter};
    auto str2 = rfir::util::Util::BitsToString(bits, 3);
    auto str3 = rfir::util::Util::BytesToString(raw + 4, 4);
    auto str4 = rfir::util::Util::BytesToString(raw + 8, 4);
    auto str5 = str2;
    auto str6 = rfir::util::Util::BytesToString(raw + 12, 4);

    String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}, {'data': '%data4%'}, {'data': '%data5%'}, {'data': '%data6%'}]";
    str.replace("'", "\"");
    str.replace("%data1%", str1.c_str());
    str.replace("%data2%", str2.c_str());
    str.replace("%data3%", str3.c_str());
    str.replace("%data4%", str4.c_str());
    str.replace("%data5%", str5.c_str());
    str.replace("%data6%", str6.c_str());
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());

    auto rfir = rfir::GetRfir(this->name);
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;
    return encode->result;    
}

bool rfir::module::device::ac::Kelvinator::onCmd_set(neb::CJsonObject* pld) {
    std::string key;

    if (!pld || !pld->GetKey(key))
        return 0;
    
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
    mode =  modeStr == "auto" ? kKelvinatorAuto : 
            modeStr == "cool" ? kKelvinatorCool : 
            modeStr == "dry" ? kKelvinatorDry : 
            modeStr == "fan" ? kKelvinatorFan : 
            modeStr == "fan_only" ? kKelvinatorFan : 
            modeStr == "heat" ? kKelvinatorHeat : mode;

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? kKelvinatorFanAuto : 
            fanStr == "low" ? kKelvinatorFanMin : 
            fanStr == "mediumLow" ? kKelvinatorFanMin + 1 : 
            fanStr == "medium" ? kKelvinatorFanMin + 2 : 
            fanStr == "mediumHigh" ? kKelvinatorFanMin + 3 : 
            fanStr == "high" ? kKelvinatorFanMax : fan;

    //Temp
    uint32 temp = ac->getTemp();
    pld->Get("temperature", temp);            

    // //Sleep
    // auto sleep = ac->getSleep();
    // std::string sleepStr;
    // pld->Get("sleep", sleepStr);
    // sleep = sleepStr == "on" ? true : 
    //         sleepStr == "off" ? false : sleep;

    //swingHor
    auto swingHor = ac->getSwingHorizontal();
    std::string swingHorStr;
    pld->Get("swingHor", swingHorStr);
    swingHor =  swingHorStr == "on" ? true : 
                swingHorStr == "off" ? false : swingHor;

    //swingVert
    auto swingVert = ac->getSwingVertical();
    std::string swingVertStr;
    pld->Get("swingVert", swingVertStr);
    swingVert =  swingVertStr == "on" ? true : 
                swingVertStr == "off" ? false : swingVert;                

    //quiet
    auto quiet = ac->getQuiet();
    std::string quietStr;
    pld->Get("quiet", quietStr);
    quiet = quietStr == "on" ? true : 
            quietStr == "off" ? false : true;

    //light
    auto light = ac->getLight();
    std::string lightStr;
    pld->Get("light", lightStr);
    light = lightStr == "on" ? true : 
            lightStr == "off" ? false : true;

    //turbo
    auto turbo = ac->getTurbo();
    std::string turboStr;
    pld->Get("turbo", turboStr);
    turbo = turboStr == "on" ? true : 
            turboStr == "off" ? false : true;




    ac->setPower(power);
    ac->setMode(mode);
    ac->setFan(fan);
    ac->setTemp(temp);
    // ac->setSleep(sleep);
    ac->setSwingHorizontal(swingHor);
    ac->setSwingVertical(swingVert);
    ac->setQuiet(quiet);
    ac->setLight(light);
    ac->setTurbo(turbo);

    ac->send();

    this->setRaw(ac->getRaw());

    return true;
}


bool rfir::module::device::ac::Kelvinator::onCmd_get(neb::CJsonObject* pld) {
    //Power
    pld->Add("power", ac->getPower() ? "on" : "off");

    //Mode
    auto mode = ac->getMode();
    std::string modeStr =   mode == kKelvinatorAuto ? "auto" :
                            mode == kKelvinatorCool ? "cool" : 
                            mode == kKelvinatorDry ? "dry" : 
                            mode == kKelvinatorFan ? "fan" : 
                            mode == kKelvinatorFan ? "fan_only" : 
                            mode == kKelvinatorHeat ? "heat" : std::string(String(mode).c_str());
    pld->Add("mode", modeStr);

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr =    fan == kKelvinatorFanAuto ? "auto" : 
                            fan == kKelvinatorFanMin ? "low" : 
                            fan == kKelvinatorFanMin + 1 ? "mediumLow" : 
                            fan == kKelvinatorFanMin + 2 ? "medium" : 
                            fan == kKelvinatorFanMin + 3 ? "mediumHigh" :                             
                            fan == kKelvinatorFanMax ? "high" : std::string(String(fan).c_str());
    pld->Add("fanSpeed", fanStr);

    //Temp
    pld->Add("temperature", ac->getTemp());    

    //Sleep
    // pld->Add("sleep", "off");

    //SwingH
    pld->Add("swingHor", ac->getSwingHorizontal() ? "on" : "off");
    //SwingV
    pld->Add("swingVert", ac->getSwingVertical() ? "on" : "off");
    //Quiet
    pld->Add("quiet", ac->getQuiet() ? "on" : "off");
    //Light
    pld->Add("light", ac->getLight() ? "on" : "off");
    //turbo
    pld->Add("turbo", ac->getTurbo() ? "on" : "off"); 

    return true;
}

bool rfir::module::device::ac::Kelvinator::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count == 3) {
        int count = 0;
        auto raw = getRaw(count);
        memcpy(raw, data->result[0].bytes, 4);
        memcpy(raw + 3, data->result[2].bytes, 4);
        return setRaw(raw);
    }
    return false;
}


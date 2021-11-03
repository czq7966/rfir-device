#include "mcquay.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::Mcquay::init() {
    this->name = "mcquay";
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

    dp[0].tolerance = 40;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 64;
    dp[0].headermark = 4690;
    dp[0].headerspace = 2610;
    dp[0].onemark = 370;
    dp[0].onespace = 910;
    dp[0].zeromark = 370;
    dp[0].zerospace = 390;
    dp[0].footermark = 370;
    dp[0].footerspace = 20470;
    dp[0].lastspace = 0;

    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    //发送参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;    
    
    return d;    
}



rfir::module::device::ac::Mcquay::Mcquay() {
    this->ac = new McquayAC(); 

}

rfir::module::device::ac::Mcquay::~Mcquay() {
    delete this->ac;
    this->ac = 0;
}


void rfir::module::device::ac::Mcquay::start(void * p) {
    Device::start(p);

    gpioCool->setArg((void*)(int)(EModePin::COOL));
    gpioDry->setArg((void*)(int)(EModePin::DRY));
    gpioFan->setArg((void*)(int)(EModePin::FAN));
    gpioHeat->setArg((void*)(int)(EModePin::HEAT));

    gpioCool->init(EModePin::COOL, INPUT);
    gpioDry->init(EModePin::DRY, INPUT);
    gpioFan->init(EModePin::FAN, INPUT);
    gpioHeat->init(EModePin::HEAT, INPUT);

    gpioCool->onChange = OnModePinChange;
    gpioDry->onChange = OnModePinChange;
    gpioFan->onChange = OnModePinChange;
    gpioHeat->onChange = OnModePinChange;

    gpioCool->start();
    gpioDry->start();
    gpioFan->start();
    gpioHeat->start();

}


void rfir::module::device::ac::Mcquay::loop() {

}

bool rfir::module::device::ac::Mcquay::setRaw(uint8_t* raw) {
    this->ac->setRaw(raw);
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Mcquay::getRaw(int& count) {
    // count = kGreeStateLength;
    return this->ac->getRaw();
}

uint16_t* rfir::module::device::ac::Mcquay::getEncodeRaw(int& count) {
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
    
    neb::CJsonObject blocks;
    blocks.Parse(str.c_str());

    auto rfir = rfir::GetRfir(this->name);
    rfir->encoder->encode(&blocks);
    auto encode = rfir->encoder->getEncodeResult();    
    count = encode->count;
    return encode->result;    
}

bool rfir::module::device::ac::Mcquay::onCmd_set(neb::CJsonObject* pld) {
    std::string key;

    // if (!pld || !pld->GetKey(key))
    //     return 0;
    

    // //Power
    // auto power = ac->getPower();
    // std::string powerStr;
    // pld->Get("power", powerStr);
    // power = powerStr == "on" ? true : 
    //         powerStr == "off" ? false : true;

    // //Mode
    // auto mode = ac->getMode();
    // std::string modeStr;
    // pld->Get("mode", modeStr);
    // mode =  modeStr == "auto" ? kGreeAuto : 
    //         modeStr == "cool" ? kGreeCool : 
    //         modeStr == "dry" ? kGreeDry : 
    //         modeStr == "fan" ? kGreeFan : 
    //         modeStr == "fan_only" ? kGreeFan : 
    //         modeStr == "heat" ? kGreeHeat : mode;

    // //FanSpeed
    // auto fan = ac->getFan();
    // std::string fanStr;
    // pld->Get("fanSpeed", fanStr);
    // fan =   fanStr == "auto" ? kGreeFanAuto : 
    //         fanStr == "low" ? kGreeFanMin : 
    //         fanStr == "medium" ? kGreeFanMed : 
    //         fanStr == "high" ? kGreeFanMax : fan;

    // //Sleep
    // auto sleep = ac->getSleep();
    // std::string sleepStr;
    // pld->Get("sleep", sleepStr);
    // sleep = sleepStr == "on" ? true : 
    //         sleepStr == "off" ? false : sleep;

    // //Swing
    // auto swing = ac->getSwingVerticalAuto();
    // std::string swingStr;
    // pld->Get("swing", swingStr);
    // swing = swingStr == "on" ? true : 
    //         swingStr == "off" ? false : swing;

    // //Temp
    // uint32 temp = ac->getTemp();
    // pld->Get("temperature", temp);


    // ac->setMode(mode);
    // ac->setFan(fan);
    // ac->setSleep(sleep);
    // ac->setSwingVertical(swing, kGreeSwingAuto);
    // ac->setTemp(temp);
    // ac->setPower(power);
    // this->setRaw(ac->getRaw());

    return true;
}


bool rfir::module::device::ac::Mcquay::onCmd_get(neb::CJsonObject* pld) {
    //Power
    // pld->Add("power", ac->getPower() ? "on" : "off");

    // //Mode
    // auto mode = ac->getMode();
    // std::string modeStr =   mode == kGreeAuto ? "auto" :
    //                         mode == kGreeCool ? "cool" : 
    //                         mode == kGreeDry ? "dry" : 
    //                         mode == kGreeFan ? "fan" : 
    //                         mode == kGreeFan ? "fan_only" : 
    //                         mode == kGreeHeat ? "heat" : std::string(String(mode).c_str());
    // pld->Add("mode", modeStr);

    // //FanSpeed
    // auto fan = ac->getFan();
    // std::string fanStr =    fan == kGreeFanAuto ? "auto" : 
    //                         fan == kGreeFanMin ? "low" : 
    //                         fan == kGreeFanMed ? "medium" : 
    //                         fan == kGreeFanMax ? "high" : std::string(String(fan).c_str());
    // pld->Add("fanSpeed", fanStr);

    // //Sleep
    // pld->Add("sleep", ac->getSleep() ? "on" : "off");

    // //Swing
    // pld->Add("swing", ac->getSwingVerticalAuto() ? "on" : "off");

    // //Temp
    // pld->Add("temperature", ac->getTemp());

    return true;
}

bool rfir::module::device::ac::Mcquay::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count == 3) {
        int count = 0;
        auto raw = getRaw(count);
        memcpy(raw, data->result[0].bytes, 4);
        memcpy(raw + 3, data->result[2].bytes, 4);
        return setRaw(raw);
    }
    return false;
}


void rfir::module::device::ac::Mcquay::doModePinChange() {
    // if (G_OnModePinChange_GpioPin != 0) {
    //     int  gpioPin = G_OnModePinChange_GpioPin;
    //     G_OnModePinChange_GpioPin = 0;

    //     auto pinMode = GetPinMode();
    //     auto reason = "Mode Change: gpioMode(" + String(GetPinModeStr(gpioPin).c_str()) + ") -> pinMode(" + String(GetModeStr(pinMode).c_str())+")"; 
    //     service::cmds::Cmd::OnCmd_get(0, std::string(reason.c_str()));    
    // }
}    

void rfir::module::device::ac::Mcquay::OnModePinChange(rfir::module::ttl::Gpio* gpio, int value) {
    // G_OnModePinChange_GpioPin = (int)gpio->getArg();
}   
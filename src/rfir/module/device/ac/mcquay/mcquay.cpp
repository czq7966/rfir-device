#include "mcquay.h"


int rfir::module::device::ac::Mcquay::ModePinChange_GpioPin = 0;

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

    // gpioCool = new rfir::module::ttl::Gpio();
    // gpioDry = new rfir::module::ttl::Gpio();
    // gpioFan = new rfir::module::ttl::Gpio();
    // gpioHeat = new rfir::module::ttl::Gpio();

    gpioCool.setArg((void*)(int)(EModePin::COOL));
    gpioDry.setArg((void*)(int)(EModePin::DRY));
    gpioFan.setArg((void*)(int)(EModePin::FAN));
    gpioHeat.setArg((void*)(int)(EModePin::HEAT));

    gpioCool.init(EModePin::COOL, INPUT);
    gpioDry.init(EModePin::DRY, INPUT);
    gpioFan.init(EModePin::FAN, INPUT);
    gpioHeat.init(EModePin::HEAT, INPUT);

    gpioCool.onChange = OnModePinChange;
    gpioDry.onChange = OnModePinChange;
    gpioFan.onChange = OnModePinChange;
    gpioHeat.onChange = OnModePinChange;

    gpioCool.start();
    gpioDry.start();
    gpioFan.start();
    gpioHeat.start();

}


void rfir::module::device::ac::Mcquay::loop() {
    doModePinChange();
    Device::loop();
}

bool rfir::module::device::ac::Mcquay::setRaw(uint8_t* raw) {
    this->ac->setRaw(raw);
    this->onSetRaw();
    return 1;
}

uint8_t* rfir::module::device::ac::Mcquay::getRaw(int& count) {
    count = McquayAC::KMcQuayStateLength;
    return this->ac->getRaw();
}

uint16_t* rfir::module::device::ac::Mcquay::getEncodeRaw(int& count) {
    count = McquayAC::KMcQuayEncodeRawLength;
    return ac->getEncodeRaw();
}

bool rfir::module::device::ac::Mcquay::onCmd_set(neb::CJsonObject* pld) {
    if (!pld ) return 0;

    //Power
    auto power = getPower();
    std::string powerStr;
    pld->Get("power", powerStr);
    power = powerStr == "on" ? true : 
            powerStr == "off" ? false : true;

    //Mode
    auto mode = ac->getMode();
    std::string modeStr;
    pld->Get("mode", modeStr);
    mode =  modeStr == "cool" ? McquayAC::KMcQuayModeCool : 
            modeStr == "dry" ? McquayAC::KMcQuayModeDry : 
            modeStr == "fan" ? McquayAC::KMcQuayModeFan : 
            modeStr == "fan_only" ? McquayAC::KMcQuayModeFan : 
            modeStr == "heat" ? McquayAC::KMcQuayModeHeat : mode;

    //FanSpeed
    auto fan = ac->getFan();
    std::string fanStr;
    pld->Get("fanSpeed", fanStr);
    fan =   fanStr == "auto" ? McquayAC::KMcQuayFanAuto : 
            fanStr == "low" ? McquayAC::KMcQuayFanMin : 
            fanStr == "medium" ? McquayAC::KMcQuayFanMed : 
            fanStr == "high" ? McquayAC::KMcQuayFanMax : fan;

    //Sleep
    auto sleep = ac->getSleep();
    std::string sleepStr;
    pld->Get("sleep", sleepStr);
    sleep = sleepStr == "on" ? true : 
            sleepStr == "off" ? false : sleep;

    //Swing
    auto swing = ac->getSwing();
    std::string swingStr;
    pld->Get("swing", swingStr);
    swing = swingStr == "on" ? true : 
            swingStr == "off" ? false : swing;

    //Temp
    uint32 temp = ac->getTemp();
    pld->Get("temperature", temp);

    //小时
    uint32 hour = ac->getHour();
    if (!pld->Get("hour", hour)) 
        hour = millis() / 1000 / 60 / 60 % 24;    
    //分钟
    uint32 minute = ac->getMinute();
    if (!pld->Get("minute", minute))
        minute = millis() / 1000 / 60 % 60;


    ac->setMode(mode);
    ac->setFan(fan);
    ac->setSleep(sleep);
    ac->setSwing(swing);
    ac->setTemp(temp);
    ac->setHour(hour);
    ac->setMinute(minute);
    ac->setPowerSwitch(power != getPower());
    
    this->setRaw(ac->getRaw());

    return true;
}


bool rfir::module::device::ac::Mcquay::onCmd_get(neb::CJsonObject* pld) {
    auto mode = getPinMode();
    std::string modeStr =  getModeStr(mode);
    auto fan = ac->getFan();
    std::string fanStr =  getFanStr(fan);

    std::string sleepStr = ac->getSleep() ? "on" : "off";
    std::string swingStr = ac->getSwing() ? "on" : "off";
    std::string powerStr = getPower() ? "on" : "off";
    std::string timerStr = getPinTimer() ? "on" : "off";

    pld->Add("power", powerStr);
    pld->Add("mode", modeStr);
    pld->Add("fanSpeed", fanStr);
    pld->Add("sleep", sleepStr);
    pld->Add("swing", swingStr);
    pld->Add("temperature", ac->getTemp());
    pld->Add("hour", ac->getHour());
    pld->Add("minute", ac->getMinute());
    pld->Add("timer", timerStr);
    pld->Add("extra", ac->toBitString() + "," + ac->toHexString());

    return true;
}

bool rfir::module::device::ac::Mcquay::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    if (data->count >= 1) {
        int size = 0;
        auto raw = getRaw(size);
        memcpy(raw, data->result[0].bytes, size);;
        return setRaw(raw);
    }
    return false;
}


bool rfir::module::device::ac::Mcquay::doModePinChange() {
    if (ModePinChange_GpioPin != 0) {
        int  gpioPin = ModePinChange_GpioPin;
        ModePinChange_GpioPin = 0;

        auto pinMode = getPinMode();
        auto reason = "Mode Change: gpioMode(" + String(getPinModeStr(gpioPin).c_str()) + ") -> pinMode(" + String(getModeStr(pinMode).c_str())+")"; 
        emitChange(reason.c_str());
        return true;
    }
    return false;
}    

bool rfir::module::device::ac::Mcquay::getPower() {
    return getPinMode() != McquayAC::KMcQuayModeNone;
}

uint8_t rfir::module::device::ac::Mcquay::getPinMode() {
    uint8_t mode = McquayAC::KMcQuayModeNone;
    
    if (digitalRead(EModePin::COOL)) mode = mode | McquayAC::KMcQuayModeCool;
    // if (digitalRead(EModePin::DRY)) mode = mode | McquayAC::KMcQuayModeDry;
    if (digitalRead(EModePin::FAN)) mode = mode | McquayAC::KMcQuayModeFan;
    if (digitalRead(EModePin::HEAT)) mode = mode | McquayAC::KMcQuayModeHeat;
    
    return mode;
}

bool rfir::module::device::ac::Mcquay::getPinTimer() {
    return digitalRead(EModePin::DRY);
}

std::string rfir::module::device::ac::Mcquay::getPinModeStr(int pin) {    
    std::string modeStr =   pin == EModePin::DRY ? "dry" :
                            pin == EModePin::COOL ? "cool" :
                            pin == EModePin::FAN ? "fan" :
                            pin == EModePin::HEAT ? "heat" : String(pin).c_str();
    return modeStr;

}
std::string rfir::module::device::ac::Mcquay::getModeStr(uint8_t mode) {
    std::string modeStr =   mode == McquayAC::KMcQuayModeDry ? "dry" :
                            mode == McquayAC::KMcQuayModeCool ? "cool" :
                            mode == McquayAC::KMcQuayModeFan ? "fan" :
                            mode == McquayAC::KMcQuayModeHeat ? "heat" : String(mode).c_str();
    return modeStr;
}

std::string rfir::module::device::ac::Mcquay::getFanStr(uint8_t fan) {
    std::string fanStr =    fan == McquayAC::KMcQuayFanAuto ? "auto" :
                            fan == McquayAC::KMcQuayFanMin ? "low" :
                            fan == McquayAC::KMcQuayFanMed ? "medium" :
                            fan == McquayAC::KMcQuayFanMax ? "high" : "unknown";
    return fanStr;
}

void rfir::module::device::ac::Mcquay::OnModePinChange(rfir::module::ttl::Gpio* gpio, int value) {
    ModePinChange_GpioPin = (int)gpio->getArg();
}   
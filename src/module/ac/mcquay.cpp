#include "mcquay.h"


module::ac::McquayAC* module::ac::Mcquay::AC = new McquayAC();
rfir::module::ttl::Gpio* module::ac::Mcquay::GpioCool = new rfir::module::ttl::Gpio();
rfir::module::ttl::Gpio* module::ac::Mcquay::GpioDry = new rfir::module::ttl::Gpio();
rfir::module::ttl::Gpio* module::ac::Mcquay::GpioFan = new rfir::module::ttl::Gpio();
rfir::module::ttl::Gpio* module::ac::Mcquay::GpioHeat = new rfir::module::ttl::Gpio();

void module::ac::Mcquay::Start() {
    AC->onSetRaw = OnSetRaw;
    InitRaw();
    GpioCool->setArg((void*)(int)(EModePin::COOL));
    GpioDry->setArg((void*)(int)(EModePin::DRY));
    GpioFan->setArg((void*)(int)(EModePin::FAN));
    GpioHeat->setArg((void*)(int)(EModePin::HEAT));

    GpioCool->init(EModePin::COOL, INPUT);
    GpioDry->init(EModePin::DRY, INPUT);
    GpioFan->init(EModePin::FAN, INPUT);
    GpioHeat->init(EModePin::HEAT, INPUT);

    GpioCool->onChange = OnModePinChange;
    GpioDry->onChange = OnModePinChange;
    GpioFan->onChange = OnModePinChange;
    GpioHeat->onChange = OnModePinChange;

    GpioCool->start();
    GpioDry->start();
    GpioFan->start();
    GpioHeat->start();
    // pinMode(EModePin::COOL, INPUT);
    // pinMode(EModePin::DRY, INPUT);
    // pinMode(EModePin::FAN, INPUT);
    // pinMode(EModePin::HEAT, INPUT);
    // attachInterrupt(EModePin::COOL, OnModePinChange_Cool, CHANGE);
    // attachInterrupt(EModePin::DRY, OnModePinChange_Dry, CHANGE);
    // attachInterrupt(EModePin::FAN, OnModePinChange_Fan, CHANGE);
    // attachInterrupt(EModePin::HEAT, OnModePinChange_Heat, CHANGE);
}

bool module::ac::Mcquay::Loop() {
    DoModePinChange();
    //Todo
    DoTimerReport();

    return false;    
}


bool module::ac::Mcquay::GetPower() {
    return GetPinMode() != module::ac::McquayAC::KMcQuayModeNone;
}

uint8_t module::ac::Mcquay::GetPinMode() {
    uint8_t mode = module::ac::McquayAC::KMcQuayModeNone;
    
    if (digitalRead(EModePin::COOL)) mode = mode | module::ac::McquayAC::KMcQuayModeCool;
    if (digitalRead(EModePin::DRY)) mode = mode | module::ac::McquayAC::KMcQuayModeDry;
    if (digitalRead(EModePin::FAN)) mode = mode | module::ac::McquayAC::KMcQuayModeFan;
    if (digitalRead(EModePin::HEAT)) mode = mode | module::ac::McquayAC::KMcQuayModeHeat;
    
    return mode;
}

std::string module::ac::Mcquay::GetPinModeStr(int pin) {
    
    std::string modeStr =   pin == EModePin::DRY ? "dry" :
                            pin == EModePin::COOL ? "cool" :
                            pin == EModePin::FAN ? "fan" :
                            pin == EModePin::HEAT ? "heat" : String(pin).c_str();
    return modeStr;

}
std::string module::ac::Mcquay::GetModeStr(uint8_t mode) {
    std::string modeStr =   mode == module::ac::McquayAC::KMcQuayModeDry ? "dry" :
                            mode == module::ac::McquayAC::KMcQuayModeCool ? "cool" :
                            mode == module::ac::McquayAC::KMcQuayModeFan ? "fan" :
                            mode == module::ac::McquayAC::KMcQuayModeHeat ? "heat" : String(mode).c_str();
    return modeStr;
}

std::string module::ac::Mcquay::GetFanStr(uint8_t fan) {
    std::string fanStr =    fan == module::ac::McquayAC::KMcQuayFanAuto ? "auto" :
                            fan == module::ac::McquayAC::KMcQuayFanMin ? "low" :
                            fan == module::ac::McquayAC::KMcQuayFanMed ? "medium" :
                            fan == module::ac::McquayAC::KMcQuayFanMax ? "high" : "unknown";
    return fanStr;
}

std::string module::ac::Mcquay::ToString() {
    auto mode = GetPinMode();
    std::string modeStr =   GetModeStr(mode);
    auto fan = AC->getFan();
    std::string fanStr =  GetFanStr(fan);

    std::string sleepStr =    AC->getSleep() ? "on" : "off";
    std::string swingStr =    AC->getSwing() ? "on" : "off";
    std::string powerStr = GetPower() ? "on" : "off";

    neb::CJsonObject json;
    json.Add("power", powerStr);
    json.Add("mode", modeStr);
    json.Add("fanSpeed", fanStr);
    json.Add("sleep", sleepStr);
    json.Add("swing", swingStr);
    json.Add("temperature", AC->getTemp());
    json.Add("hour", AC->getHour());
    json.Add("minute", AC->getMinute());
    json.Add("extra", AC->toBitString() + "," + AC->toHexString());
    return json.ToString();
}

static int G_OnModePinChange_GpioPin = 0;
void module::ac::Mcquay::OnModePinChange(rfir::module::ttl::Gpio* gpio, int value) {
    G_OnModePinChange_GpioPin = (int)gpio->getArg();
}       

void module::ac::Mcquay::OnModePinChange_Cool() {
    G_OnModePinChange_GpioPin = EModePin::COOL;
}

void module::ac::Mcquay::OnModePinChange_Dry() {
    G_OnModePinChange_GpioPin = EModePin::DRY;
}

void module::ac::Mcquay::OnModePinChange_Fan() {
    G_OnModePinChange_GpioPin = EModePin::FAN;
}

void module::ac::Mcquay::OnModePinChange_Heat() {
    G_OnModePinChange_GpioPin = EModePin::HEAT;
}

static int G_DoTimerReport_LastTime = 0;
static int G_DoTimerReport_Interval = 60 * 1000;
void module::ac::Mcquay::DoTimerReport(bool reset) {
    if (reset) {
        G_DoTimerReport_Interval = 1 * 1000;
        G_DoTimerReport_LastTime =  millis();
    }


    if (millis() - G_DoTimerReport_LastTime > G_DoTimerReport_Interval) {
        service::cmds::Cmd::OnCmd_ac_get(0, "Timer Report");

        if (G_DoTimerReport_Interval == 1 * 1000) 
            G_DoTimerReport_Interval = 2 * 1000;
        else 
            G_DoTimerReport_Interval = 60 * 1000;
        
        G_DoTimerReport_LastTime = millis();
    }

}

void module::ac::Mcquay::DoModePinChange() {
    if (G_OnModePinChange_GpioPin != 0) {
        int  gpioPin = G_OnModePinChange_GpioPin;
        G_OnModePinChange_GpioPin = 0;

        auto pinMode = GetPinMode();
        auto reason = "Mode Change: gpioMode(" + String(GetPinModeStr(gpioPin).c_str()) + ") -> pinMode(" + String(GetModeStr(pinMode).c_str())+")"; 
        service::cmds::Cmd::OnCmd_ac_get(0, std::string(reason.c_str()));    
    }
}       

void module::ac::Mcquay::OnSetRaw(void*) {
    neb::CJsonObject json;
    std::string fn = "/config.json";
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        json.Parse(context);
    }
    json.ReplaceAdd("raw", "0x" + AC->toHexString());
    context = json.ToString();
    file.writeString(context);
}

bool module::ac::Mcquay::InitRaw() {
    neb::CJsonObject json;
    std::string fn = "/config.json";
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        json.Parse(context);
        std::string raw;
        if (json.Get("raw", raw)) {
            rfir::module::ttl::Encoder::parseData(String(raw.c_str()), AC->getRaw(), false);
        }
    } else {

    }  
    return false;
}
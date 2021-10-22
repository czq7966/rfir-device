#include "mcquay.h"


module::ac::McquayAC* module::ac::Mcquay::AC = new McquayAC();

bool module::ac::Mcquay::getPower() {
    return getPinMode() != module::ac::McquayAC::KMcQuayModeNone;
}

uint8_t module::ac::Mcquay::getPinMode() {
    uint8_t mode = module::ac::McquayAC::KMcQuayModeNone;
    
    if (digitalRead(EModePin::COOL)) mode = mode | module::ac::McquayAC::KMcQuayModeCool;
    if (digitalRead(EModePin::DRY)) mode = mode | module::ac::McquayAC::KMcQuayModeDry;
    if (digitalRead(EModePin::FAN)) mode = mode | module::ac::McquayAC::KMcQuayModeFan;
    if (digitalRead(EModePin::HEAT)) mode = mode | module::ac::McquayAC::KMcQuayModeHeat;
    
    return mode;
}


std::string module::ac::Mcquay::toString() {
    auto mode = getPinMode();
    std::string modeStr =   mode == module::ac::McquayAC::KMcQuayModeDry ? "dry" :
                            mode == module::ac::McquayAC::KMcQuayModeCool ? "cool" :
                            mode == module::ac::McquayAC::KMcQuayModeFan ? "fan" :
                            mode == module::ac::McquayAC::KMcQuayModeHeat ? "heat" : String(mode).c_str();
    auto fan = AC->getFan();
    std::string fanStr =    fan == module::ac::McquayAC::KMcQuayFanAuto ? "auto" :
                            fan == module::ac::McquayAC::KMcQuayFanMin ? "low" :
                            fan == module::ac::McquayAC::KMcQuayFanMed ? "medium" :
                            fan == module::ac::McquayAC::KMcQuayFanMax ? "high" : "unknown";

    std::string sleepStr =    AC->getSleep() ? "on" : "off";
    std::string swingStr =    AC->getSwing() ? "on" : "off";
    uint8_t temp = AC->getTemp();    
    bool powerStr = getPower() ? "on" : "off";
    neb::CJsonObject json;
    json.Add("power", powerStr);
    json.Add("mode", modeStr);
    json.Add("fanSpeed", fanStr);
    json.Add("sleep", sleepStr);
    json.Add("swing", swingStr);
    json.Add("temperature", temp);
    json.Add("extra", AC->toBitString() + "," + AC->toHexString());
    return json.ToString();
}
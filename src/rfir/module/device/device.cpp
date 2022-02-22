#include "device.h"
#include "rfir/rfir.h"


rfir::module::ttl::Config::Device* rfir::module::device::Device::init() {
    return 0;
}

void rfir::module::device::Device::emitChange(const char* reason) {
    if (onChange)
        onChange(this, reason);
}

void rfir::module::device::Device::start(void *) {
    loadRaw();   
}

void rfir::module::device::Device::loop() {
    doTimerReport();
    doRawChanged();
}


bool rfir::module::device::Device::onCmd_set(neb::CJsonObject* pld) {
    return 0;
}

bool rfir::module::device::Device::onCmd_get(neb::CJsonObject* pld) {
    return 0;
}

bool rfir::module::device::Device::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    return 0;
}

bool rfir::module::device::Device::setRaw(uint8_t* raw) {
    return 0;
}

uint8_t* rfir::module::device::Device::getRaw(int& count) {
    return 0;
}

uint16_t* rfir::module::device::Device::getEncodeRaw(int& count) {
    return 0;
}

void rfir::module::device::Device::onSetRaw() {
    // saveRaw();
    rawChanged = true;
}


bool rfir::module::device::Device::saveRaw() {
    neb::CJsonObject json;
    std::string fn = "/"+ this->name + "-raw.json";
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        json.Parse(context);
    }

    json.ReplaceAdd("raw", "0x" + toHexString());
    context = json.ToString();
    return file.writeString(context);
}

bool rfir::module::device::Device::loadRaw() {
    neb::CJsonObject json;
    std::string fn = "/"+ this->name + "-raw.json";
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);
    if (context.length() > 0) {
        json.Parse(context);
        std::string rawStr;
        if (json.Get("raw", rawStr)) {
            int count = 0;
            auto raw = getRaw(count);
            return rfir::module::ttl::Encoder::parseData(String(rawStr.c_str()), raw, false);
        }
    } else {

    }  
    return false;

}


void rfir::module::device::Device::doTimerReport(bool reset) {
    if (reset) {
        reinitTimerReport(reset);
    }


    if (millis() - timerReport_LastTime > timerReport_Interval) {
        emitChange("Timer Report");

        if (timerReport_Interval == 1 * 1000) 
            timerReport_Interval = 2 * 1000;
        else 
            timerReport_Interval = 60 * 1000;
        
        timerReport_LastTime = millis();
    }
}

void rfir::module::device::Device::reinitTimerReport(bool reset) {
    if (reset) {
        timerReport_Interval = 1 * 1000;
    } else {
        timerReport_Interval = 60 * 1000;
    }    

    timerReport_LastTime =  millis();    
}

void rfir::module::device::Device::doRawChanged() {
    if (rawChanged) {
        rawChanged = false;
        saveRaw();
    }
}

std::string rfir::module::device::Device::toBitString() {
    int count = 0;
    auto raw = getRaw(count);
    return rfir::util::Util::BytesToString(raw, count);
}

std::string rfir::module::device::Device::toHexString() {
    int count = 0;
    auto raw = getRaw(count);
    return rfir::util::Util::BytesToHexString(raw, count);
}
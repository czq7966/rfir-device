#include "device.h"
#include "rfir/rfir.h"
#include "cmds/cmd/cmd-dispatcher.h"


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
    rawChanged_time = millis();
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
    if (rawChanged && millis() - rawChanged_time > rawChanged_timeout) {
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



//Networking
rfir::module::device::Networking::~Networking(){
    GCmdDispatcher->events.onConnect.remove((void*)this);
    GCmdDispatcher->events.onCommand.remove((void*)this);
};

void rfir::module::device::Networking::start(){
    GCmdDispatcher->events.onConnect.add((void*)this, OnConnect, (void*)this);
    GCmdDispatcher->events.onCommand.add((void*)this, OnCommand, (void*)this);
};


void rfir::module::device::Networking::loop(){
    static unsigned long timeout = millis();
    if (millis() - timeout > 500) {
        if (m_logined)
            login();
        timeout = millis();
    }
}


void rfir::module::device::Networking::login(){
    DEBUGER.println("rfir::module::device::Networking::login");
    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    cmd.respTimeout = 500;
    // cmd.events.onResp.callback = OnLoginResp;
    // cmd.events.onResp.cbArg = (void*) this;
    // cmd.events.onTimeout.callback = OnLoginTimeout;
    // cmd.events.onTimeout.cbArg = (void*) this;


    cmd.topic = Config.mqtt_dsp_svc_login;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    pld.Add("id", Config.dev_id);
    pld.Add("version", OTA_VERSION_NUMBER);
    pld.Add("rssi", WiFi.RSSI());
    pld.Add("ssid", WiFi.SSID().c_str());
    pld.Add("ip", WiFi.localIP().toString().c_str());
    pld.Add("mac", rfir::util::Util::GetMacAddress());
    pld.Add("facturer", DEV_FACTURER);
    pld.Add("model", DEV_MODEL);    
    cmd.send();
};
void rfir::module::device::Networking::handshake(){};
void rfir::module::device::Networking::heartbeat(){};

void* rfir::module::device::Networking::OnConnect(void* arg, void* p){
    auto networking = (rfir::module::device::Networking*) arg;
    if (!networking->m_logined) {
        networking->m_logined = true;
        // networking->login();
    }
    return 0;
};

void* rfir::module::device::Networking::OnCommand(void* arg, void* p){
    return 0;
};

void* rfir::module::device::Networking::OnLoginResp(void* arg, void* p){

    return 0;
};

void* rfir::module::device::Networking::OnLoginTimeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::OnLoginTimeout");
    auto networking = (rfir::module::device::Networking*) arg;
    networking->login();

    return 0;
};
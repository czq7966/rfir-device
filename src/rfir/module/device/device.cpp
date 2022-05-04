#include "device.h"
#include "rfir/rfir.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"


rfir::module::ttl::Config::Device* rfir::module::device::Device::init() {
    return 0;
}

void rfir::module::device::Device::emitChange(const char* reason) {
    events.onChange.emit((void*)reason);
}

void rfir::module::device::Device::start(void *) {
    loadRaw();   
}

void rfir::module::device::Device::loop() {
    doTimerReport();
    doRawChanged();
}

bool rfir::module::device::Device::getCommonProps(neb::CJsonObject* pld){
    pld->ReplaceAdd("id", Config.dev_id);
    pld->ReplaceAdd("ip", WiFi.localIP().toString().c_str());
    pld->ReplaceAdd("mac", rfir::util::Util::GetMacAddress());
    pld->ReplaceAdd("rssi", WiFi.RSSI());
    pld->ReplaceAdd("ssid", WiFi.SSID().c_str());
    pld->ReplaceAdd("version", OTA_VERSION_NUMBER);
    pld->ReplaceAdd("facturer", DEV_FACTURER);
    pld->ReplaceAdd("model", DEV_MODEL);    
    return true;
};
bool rfir::module::device::Device::getProps(neb::CJsonObject* pld){
    onCmd_get(pld);
    return getCommonProps(pld);
};

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
        this->events.onChange.emit((void*)"Timer Report");

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
    GCmdDispatcher.events.onConnect.remove((void*)this);
    GCmdDispatcher.events.onCommand.remove((void*)this);
};

void rfir::module::device::Networking::start(){
    setWill();
    GCmdDispatcher.events.onConnect.add((void*)this, std::bind(&Networking::onConnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onCommand.add((void*)this, std::bind(&Networking::onCommand, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};


void rfir::module::device::Networking::loop(){

}


bool rfir::module::device::Networking::login(){
    DEBUGER.println("rfir::module::device::Networking::login");
    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    cmd.command.head.to.type = "dsp";
    cmd.command.head.to.id = Config.dsp_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = Config.mqtt_dsp_svc_login;

    cmd.events.onResp.callback = std::bind(&Networking::onLoginResp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onLoginTimeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = (void*)this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getProps(&pld);

    return cmd.send();
};
bool rfir::module::device::Networking::handshake(){
    if (Config.edg_id == "") {
        //没有边缘服务，5秒后重新登入
        GEventTimer.delay(5000, std::bind(&Networking::doLogin, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
        return false;
    } 

    DEBUGER.println("rfir::module::device::Networking::handshake");
    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    cmd.command.head.to.type = "edg";
    cmd.command.head.to.id = Config.edg_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = Config.mqtt_edg_svc_handshake;

    cmd.events.onResp.callback = std::bind(&Networking::onHandshakeResp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onHandshakeTimeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getCommonProps(&pld);
    return cmd.send(); 
};
bool rfir::module::device::Networking::heartbeat(){
    return 0;
};

void rfir::module::device::Networking::setWill(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="";
    cmd.command.head.to.id = "";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getCommonProps(&pld);
    pld.Add("online", false);
    GMqttClient.setWill(cmd.expandTopic().c_str(), cmd.command.toString().c_str());
};

void rfir::module::device::Networking::setOnline(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="";
    cmd.command.head.to.id = "";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getCommonProps(&pld);
    pld.Add("online", true);
    GMqttClient.publish(cmd.expandTopic().c_str(), cmd.command.toString().c_str(), true);
};


// void rfir::module::device::Networking::onLoginReq(cmds::cmd::CmdBase* cmd){

// };
// void* rfir::module::device::Networking::onLoginResp(void* arg, void* p){
//     cmd->command.pld.Get("app", Config.app_id);
//     cmd->command.pld.Get("mod", Config.dom_id);
//     cmd->command.pld.Get("dsp", Config.dsp_id);
//     cmd->command.pld.Get("edg", Config.edg_id);
//     handshake(_this);

// };
// void* rfir::module::device::Networking::onLoginTimeout(void* arg, void* p){
//     // handshake();
//     login();
// };
// void rfir::module::device::Networking::onHandshakeReq(::cmds::cmd::CmdBase* cmd){

// };
// void rfir::module::device::Networking::onHandshakeResp(::cmds::cmd::CmdBase* cmd){
//     m_handshake_failed_count = 0;
// };

// void rfir::module::device::Networking::onHandshakeTimeout(uint32_t sid){    
//     m_handshake_failed_count++;
//     if (m_handshake_failed_count >= 3) {
//         //3次握手失败，重新登入
//         login();
//     } else {
//         //再次请求握手
//         handshake();
//     }    
// };

void* rfir::module::device::Networking::onConnect(void* arg, void* p){
    setOnline();
    if (!this->m_logined) {
        this->m_logined = true;
        this->login();
    }   
    return 0;
    // auto netorking = (rfir::module::device::Networking*) arg;
    // if (!netorking->m_logined) {
    //     netorking->m_logined = true;
    //     netorking->login();
    // }
    // return 0;
};

void* rfir::module::device::Networking::onCommand(void* arg, void* p){
    return 0;
};


void* rfir::module::device::Networking::doLogin(void* arg, void* p) {
    this->login();
    return 0;
};

void* rfir::module::device::Networking::onLoginResp(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::OnLoginResp");  
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    cmd->command.pld.Get("app", Config.app_id);
    cmd->command.pld.Get("mod", Config.dom_id);
    cmd->command.pld.Get("dsp", Config.dsp_id);
    cmd->command.pld.Get("edg", Config.edg_id);
    this->m_logined = Config.edg_id != "";
    handshake();     

    // rfir::module::device::Networking* netorking = (rfir::module::device::Networking*)arg;
    // netorking->onLoginResp((cmds::cmd::CmdBase*)(p), netorking);
    return 0;
};

void* rfir::module::device::Networking::onLoginTimeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::OnLoginTimeout");
    login();
    // auto netorking = (rfir::module::device::Networking*) arg;
    // netorking->onLoginTimeout((uint32_t)(p));
    return 0;
};

void* rfir::module::device::Networking::onHandshakeReq(void* arg, void* p){
    return 0;
};
void* rfir::module::device::Networking::onHandshakeResp(void* arg, void* p){
    return 0;
};               
void* rfir::module::device::Networking::onHandshakeTimeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::OnHandshakeTimeout");
    m_handshake_failed_count++;
    if (m_handshake_failed_count >= 3) {
        //3次握手失败，重新登入
        login();
    } else {
        //再次请求握手
        handshake();
    }   
    // auto netorking = (rfir::module::device::Networking*) arg;
    // netorking->onHandshakeTimeout((uint32_t)(p));
    return 0;
};
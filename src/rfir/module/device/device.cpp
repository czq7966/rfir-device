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
    Config.events.onFixup.remove((void*)this);
     GCmdDispatcher.events.onConnect.remove((void*)this);
    GCmdDispatcher.events.onConnect.remove((void*)this);
    GCmdDispatcher.events.onCommand.remove((void*)this);
};

//事件注册
void rfir::module::device::Networking::start(){
    Config.events.onFixup.add((void*)this, std::bind(&Networking::onConfigFixup, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onConnect.add((void*)this, std::bind(&Networking::onConnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onDisconnect.add((void*)this, std::bind(&Networking::onDisconnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onCommand.add((void*)this, std::bind(&Networking::onCommand, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};


void rfir::module::device::Networking::loop(){

}

//登入调度
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
    
    GDevice->getCommonProps(&pld);
    Config.getIds(&pld);

    return cmd.send();
};

//延时登入
void rfir::module::device::Networking::delayLogin(int delay_ms) {
    DEBUGER.printf("rfir::module::device::Networking::delayLogin %d \r\n", delay_ms);
    GEventTimer.delay(delay_ms, std::bind(&Networking::doLogin, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//握手
bool rfir::module::device::Networking::handshake(){
    if (Config.edg_id == "") {
        //没有边缘服务，3秒后重新登入
        delayLogin();
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
    hd.ReplaceAdd("count", m_handshake_success_count);
    return cmd.send(); 
};


//遗嘱发布
bool rfir::module::device::Networking::setWill(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.from.type ="dev";
    cmd.command.head.from.id = Config.dev_id;
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getCommonProps(&pld);
    pld.Add("online", 0);
    Config.getIds(&pld);
    
    GMqttClient.setWill(cmd.expandTopic().c_str(), cmd.command.toString().c_str());
    return 1;
};

//上线发布
void rfir::module::device::Networking::setOnline(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.from.type ="dev";
    cmd.command.head.from.id = Config.dev_id;
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    neb::CJsonObject& pld = cmd.command.pld;
    GDevice->getCommonProps(&pld);
    pld.Add("online", 1);
    Config.getIds(&pld);

    GMqttClient.publish(cmd.expandTopic().c_str(), cmd.command.toString().c_str(), true);
};

//订阅主题
void rfir::module::device::Networking::subscribe() {
    
    if (!GMqttClient.mqtt.connected())
        return;

    DEBUGER.printf("rfir::module::device::Networking::subscribe \r\n");
    std::string topic;
    topic = Config.getSvcTopic(Config.mqtt_dev_svc_login);
    GMqttClient.mqtt.unsubscribe(topic.c_str()); 
    GMqttClient.mqtt.subscribe(topic.c_str(), 2);

    topic = Config.getSvcTopic(Config.mqtt_dev_svc_handshake);
    GMqttClient.mqtt.unsubscribe(topic.c_str()); 
    GMqttClient.mqtt.subscribe(topic.c_str(), 2);

    topic = Config.getEdgStatusTopic();
    if (topic != "") {
        GMqttClient.mqtt.unsubscribe(topic.c_str()); 
        GMqttClient.mqtt.subscribe(topic.c_str(), 2);    
    }

}

//MQTT连接事件
void* rfir::module::device::Networking::onConnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onConnect \r\n");
    subscribe();
    setOnline();
    if (!this->m_logined) {
        this->m_logined = true;
        this->login();
    }   
    return 0;

};

//MQTT断线事件
void* rfir::module::device::Networking::onDisconnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onDisconnect \r\n");
    return 0;
};

//事件指令
void* rfir::module::device::Networking::onCommand(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    DEBUGER.printf("rfir::module::device::Networking::onCommand: %s , %s \r\n", cmd->topic.c_str(), Config.getEdgStatusTopic().c_str());
    if (cmd->topic == Config.getEdgStatusTopic()) {
        return onEdgSatusChange(arg, p);
    } else {
        DEBUGER.printf("rfir::module::device::Networking::onCommand: no equl");
    }
    return 0;
};

//配置改变事件
void* rfir::module::device::Networking::onConfigFixup(void* arg, void* p) {
    setWill();
    return 0;
};

//事件登入
void* rfir::module::device::Networking::doLogin(void* arg, void* p) {
    this->login();
    return 0;
};

//调度登入响应
void* rfir::module::device::Networking::onLoginResp(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;

    DEBUGER.printf("rfir::module::device::Networking::OnLoginResp: %s \r\n", cmd->command.pld.ToString().c_str());  
    
    std::string app_id, dom_id, dsp_id, edg_id;

    cmd->command.pld.Get("app", app_id);
    cmd->command.pld.Get("dom", dom_id);
    cmd->command.pld.Get("dsp", dsp_id);
    cmd->command.pld.Get("edg", edg_id);
    
    
    m_logined = app_id != "" && dom_id != "" && dsp_id !="" && edg_id != "";

    if (m_logined) {
        if (app_id != Config.app_id || dom_id != Config.dom_id || dsp_id != Config.dsp_id || edg_id != Config.edg_id){
            Config.app_id = app_id;
            Config.dom_id = dom_id;
            Config.dsp_id = dsp_id;
            Config.edg_id = edg_id;
            Config.fixup();
            subscribe();
        }   
        
        handshake();
    } else {
        delayLogin();
    }

    return 0;
};

//调度登入超时
void* rfir::module::device::Networking::onLoginTimeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::OnLoginTimeout");
    m_logined = Config.app_id != "" && Config.dom_id != "" && Config.dsp_id !="" && Config.edg_id != "";

    if (m_logined) {
        handshake();        
    } else {
        login();
    }

    return 0;
};

//边缘握手响应
void* rfir::module::device::Networking::onHandshakeResp(void* arg, void* p){
    setOnline();
    m_handshake_success_count++;
    DEBUGER.printf("rfir::module::device::Networking::onHandshakeResp: %d\r\n", m_handshake_success_count);
    
    return 0;
};               

//边缘握手超时
void* rfir::module::device::Networking::onHandshakeTimeout(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::OnHandshakeTimeout: %d\r\n", m_handshake_failed_count);
    m_handshake_failed_count++;
    login();
    return 0;
};

//边缘服务状态通知
void* rfir::module::device::Networking::onEdgSatusChange(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onEdgSatusChange");
    //握手边缘
    handshake();
    return 0;
};


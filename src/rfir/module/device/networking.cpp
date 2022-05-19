#include "networking.h"
#include "device.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"


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

    GCmdDispatcher.removeWaitResp(m_login_handler);

    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    m_login_handler = cmd.command.getIntSid();
    cmd.command.head.to.type = "dsp";
    cmd.command.head.to.id = Config.dsp_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = Config.mqtt_dsp_svc_login;

    cmd.events.onResp.callback = std::bind(&Networking::onDev_login_resp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onDev_login_timeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = (void*)this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    Config.getIds(&pld);  
    GDevice->getCommonProps(&pld);

    return cmd.send();
};

//延时登入
void rfir::module::device::Networking::delayLogin(int delay_ms) {
    DEBUGER.printf("rfir::module::device::Networking::delayLogin %d \r\n", delay_ms);
    GEventTimer.delay(delay_ms, std::bind(&Networking::doLogin, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//事件登入
void* rfir::module::device::Networking::doLogin(void* arg, void* p) {
    this->login();
    return 0;
};

//握手
bool rfir::module::device::Networking::handshake(){
    if (Config.edg_id == "") {
        //没有边缘服务，3秒后重新登入
        delayLogin();
        return false;
    } 

    GCmdDispatcher.removeWaitResp(m_handshake_handler);

    DEBUGER.println("rfir::module::device::Networking::handshake");
    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    m_handshake_handler = cmd.command.getIntSid();
    cmd.command.head.to.type = "edg";
    cmd.command.head.to.id = Config.edg_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = Config.mqtt_edg_svc_handshake;

    cmd.events.onResp.callback = std::bind(&Networking::onDev_handshake_resp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onDev_handshake_timeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&pld);
    GDevice->getCommonProps(&pld);
    hd.ReplaceAdd("handshake_count", m_handshake_success_count);

    return cmd.send(); 
};

//延时握手
void rfir::module::device::Networking::delayHandshake(int delay_ms){
    DEBUGER.printf("rfir::module::device::Networking::delayHandshake %d \r\n", delay_ms);
    GEventTimer.delay(delay_ms, std::bind(&Networking::doLogin, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//事件登入
void* rfir::module::device::Networking::doHandshake(void* arg, void* p) {
    this->handshake();
    return 0;
};


//遗嘱发布
bool rfir::module::device::Networking::setWill(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    cmd.command.head.retain = true;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&pld);
    GDevice->getCommonProps(&pld);
    pld.Add("online", 0);
    pld.Add("online_count", m_online_count + 1);
    
    GMqttClient.setWill(cmd.expandTopic().c_str(), cmd.command.toString().c_str());
    return 1;
};

//上线发布
void rfir::module::device::Networking::setOnline(){
    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = Config.mqtt_dev_evt_status;
    cmd.command.head.retain = true;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&pld);
    GDevice->getCommonProps(&pld);
    pld.Add("online", 1);
    pld.Add("online_count", m_online_count);
    cmd.send();
};

//订阅主题
void rfir::module::device::Networking::subscribe() {    
    if (!GMqttClient.mqtt.connected())
        return;

    unsubscribe();

    DEBUGER.printf("rfir::module::device::Networking::subscribe \r\n");
    //dsp
    GMqttClient.mqtt.subscribe(Config.mqtt_dsp_evt_status.c_str(), 2);

    //edg
    GMqttClient.mqtt.subscribe(Config.mqtt_edg_evt_status.c_str(), 2);

    //dev
    GMqttClient.mqtt.subscribe(Config.mqtt_dev_svc_login.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.mqtt_dev_svc_handshake.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.mqtt_dev_svc_get.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.mqtt_dev_svc_set.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.mqtt_dev_svc_penet.c_str(), 2);
}

//取消订阅
void rfir::module::device::Networking::unsubscribe() {
    if (!GMqttClient.mqtt.connected())
        return;
    DEBUGER.printf("rfir::module::device::Networking::unsubscribe \r\n");
    //dsp
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dsp_evt_status.c_str());

    //edg
    GMqttClient.mqtt.unsubscribe(Config.mqtt_edg_evt_status.c_str());

    //dev
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dev_svc_login.c_str());
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dev_svc_handshake.c_str());
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dev_svc_get.c_str());
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dev_svc_set.c_str());
    GMqttClient.mqtt.unsubscribe(Config.mqtt_dev_svc_penet.c_str());
}

//MQTT连接事件
void* rfir::module::device::Networking::onConnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onConnect \r\n");
    status.connected = true;
    subscribe();
    m_online_count++;
    setOnline();
    if (!status.logined) {
        status.logined = true;
        this->login();
    }   
    return 0;

};

//MQTT断线事件
void* rfir::module::device::Networking::onDisconnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onDisconnect \r\n");
    status.connected = false;
    return 0;
};

//事件指令
void* rfir::module::device::Networking::onCommand(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    DEBUGER.printf("rfir::module::device::Networking::onCommand: %s \r\n", cmd->topic.c_str());
    if (cmd->topic == Config.mqtt_dsp_evt_status) {
        return onDsp_status_change(arg, p);
    } 
    
    if (cmd->topic == Config.mqtt_edg_evt_status) {
        return onEdg_status_change(arg, p);
    } 
    
    if (cmd->command.head.stp == 0) {
        //握手请求
        if (cmd->topic == Config.mqtt_dev_svc_handshake) {
            handshake();
            return 0;            
        }
    }

    return 0;
};

//配置改变事件
void* rfir::module::device::Networking::onConfigFixup(void* arg, void* p) {
    setWill();
    return 0;
};


//调度登入响应
void* rfir::module::device::Networking::onDev_login_resp(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;

    DEBUGER.printf("rfir::module::device::Networking::onDev_login_resp: %s \r\n", cmd->command.pld.ToString().c_str());  
    
    std::string app_id, dom_id, dsp_id, edg_id;

    cmd->command.pld.Get("app", app_id);
    cmd->command.pld.Get("dom", dom_id);
    cmd->command.pld.Get("dsp", dsp_id);
    cmd->command.pld.Get("edg", edg_id);
    
    
    status.logined = app_id != "" && dom_id != "" && dsp_id !="" && edg_id != "";

    if (status.logined) {
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
void* rfir::module::device::Networking::onDev_login_timeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onDev_login_timeout");
    status.logined = Config.app_id != "" && Config.dom_id != "" && Config.dsp_id !="" && Config.edg_id != "";

    if (status.logined) {
        handshake();        
    } else {
        login();
    }

    return 0;
};

//边缘握手响应
void* rfir::module::device::Networking::onDev_handshake_resp(void* arg, void* p){
    status.handshaked = true;
    setOnline();
    m_handshake_success_count++;
    DEBUGER.printf("rfir::module::device::Networking::onDev_handshake_resp: %d\r\n", m_handshake_success_count);
    
    return 0;
};               

//边缘握手超时
void* rfir::module::device::Networking::onDev_handshake_timeout(void* arg, void* p){
    status.handshaked = false;
    m_handshake_failed_count++;
    DEBUGER.printf("rfir::module::device::Networking::onDev_handshake_timeout: %d\r\n", m_handshake_failed_count);    
    login();
    return 0;
};



//调度服务状态通知
void* rfir::module::device::Networking::onDsp_status_change(void* arg, void* p){
    setOnline();
    return 0;
};

//边缘服务状态通知
void* rfir::module::device::Networking::onEdg_status_change(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onEdg_status_change");
    //握手边缘
    handshake();
    return 0;
};

rfir::module::device::Networking GNetworking;
#include "networking.h"
#include "device.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/led.h"
#include "rfir/util/util.h"


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
    GCmdDispatcher.events.onConnect.add((void*)this, std::bind(&Networking::onMqttConnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onDisconnect.add((void*)this, std::bind(&Networking::onMqttDisconnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    GCmdDispatcher.events.onCommand.add((void*)this, std::bind(&Networking::onCommand, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};


void rfir::module::device::Networking::loop(){

}


//握手
bool rfir::module::device::Networking::handshake(){

    if (m_handshake_handler)
        return 1;

    GCmdDispatcher.removeWaitResp(m_handshake_handler);
    GEventTimer.remove(m_handshake_heartbeat_handler);
    m_handshake_handler = 0;
    m_handshake_heartbeat_handler = 0;

    DEBUGER.println("rfir::module::device::Networking::handshake");
    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    m_handshake_handler = cmd.command.getIntSid();
    cmd.command.head.to.type = "edg";
    cmd.command.head.to.id = Config.props.edg_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = "handshake";

    cmd.events.onResp.callback = std::bind(&Networking::onDev_handshake_resp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onDev_handshake_timeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = this;

    JsonObject& hd = cmd.command.hd;
    JsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld["handshake_count"] = m_handshake_success_count;
    pld["handshake_failed_count"] = m_handshake_failed_count;    
    pld["freeheap"] = ESP.getFreeHeap();
    pld["freestack"] = ESP.getFreeContStack();
    return cmd.send(); 
};

//延时握手
void rfir::module::device::Networking::delayHandshake(int delay_ms){
    DEBUGER.printf("rfir::module::device::Networking::delayHandshake %d \r\n", delay_ms);
    GEventTimer.remove(m_handshake_heartbeat_handler);
    m_handshake_heartbeat_handler = GEventTimer.delay(delay_ms, std::bind(&Networking::doHandshake, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//事件登入
void* rfir::module::device::Networking::doHandshake(void* arg, void* p) {
    this->handshake();
    return 0;
};


//遗嘱发布
bool rfir::module::device::Networking::setWill(){
    cmds::cmd::CmdMqtt cmd;
    cmd.prefix = "0/0/";
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = "status";
    cmd.command.head.retain = true;
    JsonObject& hd = cmd.command.hd;
    JsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld["online"] = 0;
    pld["online_count"] = m_online_count + 1;

    std::string topic = cmd.expandTopic();    
    topic = Config.props.mqtt_dev_status;
    GMqttClient.setWill(topic.c_str(), cmd.command.toString().c_str());

    return 1;
};

//上线发布
void rfir::module::device::Networking::setOnline(){
    cmds::cmd::CmdMqtt cmd;
    cmd.prefix = "0/0/";
    cmd.command.head.to.type ="0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = "status";
    cmd.command.head.retain = true;
    JsonObject& hd = cmd.command.hd;
    JsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld["online"] = 1;
    pld["online_count"] = m_online_count;
    pld["timestamp"] = (uint64_t)millis();
    pld["reboot"] = m_reboot;


    cmd.topic = Config.props.mqtt_dev_status;
    cmd.send();
    if (m_reboot) {
        pld["reboot"] = 0;
        cmd.send();
    }
    m_reboot = 0;
};

//订阅主题
void rfir::module::device::Networking::subscribe() {    
    if (!GMqttClient.mqtt.connected())
        return;

    DEBUGER.printf("rfir::module::device::Networking::subscribe %s \r\n", Config.props.mqtt_dev_sub.c_str());
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_sub.c_str(), 2);
}

//取消订阅
void rfir::module::device::Networking::unsubscribe() {
    if (!GMqttClient.mqtt.connected())
        return;    

    DEBUGER.printf("rfir::module::device::Networking::unsubscribe %s \r\n", Config.props.mqtt_dev_sub.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_sub.c_str());

}

//重置至登入前
void rfir::module::device::Networking::reset() {
    unsubscribe();
    Config.reset();
    Config.fixup();
    subscribe();

};
//MQTT连接事件
void* rfir::module::device::Networking::onMqttConnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onConnect \r\n");
    status.connected = true;
    delayNetworking();

    return 0;

};

//MQTT断线事件
void* rfir::module::device::Networking::onMqttDisconnect(void* arg, void* p){
    DEBUGER.printf("rfir::module::device::Networking::onDisconnect \r\n");
    status.connected = false;
    GLed.stop();
    return 0;
};

//组网超时
void* rfir::module::device::Networking::onNetworkingTimeout(void* arg, void* p) {
    rfir::util::Util::Reset();
    return 0;
};

//事件指令
void* rfir::module::device::Networking::onCommand(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    DEBUGER.printf("rfir::module::device::Networking::onCommand: %s \r\n", cmd->topic.c_str());
    
    if (cmd->command.head.stp == 0) {
        //握手请求
        if (cmd->command.head.entry.type == "svc") {
            if (cmd->command.head.entry.id == "handshake") {
                handshake();
                return 0;
            }
        }
    }

    return 0;
};

//配置改变事件
void* rfir::module::device::Networking::onConfigFixup(void* arg, void* p) {
    setWill();
    return 0;
};


//边缘握手响应
void* rfir::module::device::Networking::onDev_handshake_resp(void* arg, void* p){
    GLed.stop();
    status.logined = true;
    status.handshaked = true;
    m_handshake_success_count++;
    m_handshake_handler = 0;
    delayHandshake(Config.props.networking_re_handshake_timeout);
    DEBUGER.printf("rfir::module::device::Networking::onDev_handshake_resp: %d\r\n", m_handshake_success_count);
    return 0;

};               

//边缘握手超时
void* rfir::module::device::Networking::onDev_handshake_timeout(void* arg, void* p){
    status.handshaked = false;
    m_handshake_failed_count++;
    m_handshake_handler = 0;
    DEBUGER.printf("rfir::module::device::Networking::onDev_handshake_timeout: %d\r\n", m_handshake_failed_count);    
    delayHandshake(Config.props.networking_re_handshake_timeout);
    
    return 0;
};


void rfir::module::device::Networking::delayNetworking(int delay_ms){
    GEventTimer.delay(delay_ms, std::bind(&Networking::doNetworking, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

void* rfir::module::device::Networking::doNetworking(void* arg, void* p){    
    unsubscribe();
    subscribe();
    m_online_count++;
    setOnline();
 
    return 0;
};

rfir::module::device::Networking GNetworking;
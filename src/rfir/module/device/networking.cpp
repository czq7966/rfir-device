#include "networking.h"
#include "device.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/led.h"


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

void rfir::module::device::Networking::delayNetworking(int delay_ms){
    GEventTimer.delay(delay_ms, std::bind(&Networking::doNetworking, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//
void* rfir::module::device::Networking::doNetworking(void* arg, void* p){    
    unsubscribe();
    subscribe();
    m_online_count++;
    setOnline();
    if (!status.logined) {        
        this->loginDsp();
    }   
    return 0;
};

//登入调度
bool rfir::module::device::Networking::loginDsp(){
    DEBUGER.println("rfir::module::device::Networking::loginDsp");
    if (Config.props.dsp_id == "") {
        //没有边缘服务，3秒后重新登入
        delayLoginDio();
        return false;
    } 
    

    GCmdDispatcher.removeWaitResp(m_login_handler);

    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    m_login_handler = cmd.command.getIntSid();
    cmd.command.head.to.type = "dsp";
    cmd.command.head.to.id = Config.props.dsp_id;
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = "login";

    cmd.events.onResp.callback = std::bind(&Networking::onDev_login_dsp_resp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onDev_login_dsp_timeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = (void*)this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    Config.getIds(&hd);  
    GDevice->getCommonProps(&pld);

    return cmd.send();
};

//延时登入
void rfir::module::device::Networking::delayLoginDsp(int delay_ms) {
    DEBUGER.printf("rfir::module::device::Networking::delayLoginDsp %d \r\n", delay_ms);
    GEventTimer.delay(delay_ms, std::bind(&Networking::doLoginDsp, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//事件登入
void* rfir::module::device::Networking::doLoginDsp(void* arg, void* p) {
    this->loginDsp();
    return 0;
};


//登入数据服务
bool rfir::module::device::Networking::loginDio() {
    if (!status.connected) return 0;
    
    DEBUGER.println("rfir::module::device::Networking::loginDIO");
    if (GLed.idle()) GLed.start(&(NETWORKING_LOGIN_JLED));
    reset();
    GCmdDispatcher.removeWaitResp(m_login_handler);
    if (m_login_reset_handler == 0) {
        m_login_reset_handler = GEventTimer.delay(Config.props.networking_reset_timeout, std::bind(&Networking::onNetworkingTimeout, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
    }

    cmds::cmd::CmdMqtt cmd;
    cmd.command.setNeedResp();
    m_login_handler = cmd.command.getIntSid();
    cmd.command.head.to.type = "0";
    cmd.command.head.to.id = "0";
    cmd.command.head.entry.type = "svc";
    cmd.command.head.entry.id = "login";

    cmd.events.onResp.callback = std::bind(&Networking::onDev_login_dio_resp, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onResp.cbArg = (void*)this;   
    cmd.events.onTimeout.callback = std::bind(&Networking::onDev_login_dio_timeout, this, std::placeholders::_1, std::placeholders::_2);
    cmd.events.onTimeout.cbArg = (void*)this;

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    Config.getIds(&hd);  
    GDevice->getCommonProps(&pld);

    return cmd.send();
};


//延时登入数据服务
void rfir::module::device::Networking::delayLoginDio(int delay_ms) {
    DEBUGER.printf("rfir::module::device::Networking::delayLoginDio %d \r\n", delay_ms);
    GEventTimer.delay(delay_ms, std::bind(&Networking::doLoginDio, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

//事件登入数据服务
void* rfir::module::device::Networking::doLoginDio(void* arg, void* p) {
    this->loginDio();
    return 0;
};


//握手
bool rfir::module::device::Networking::handshake(){
#ifdef NETWORKING_V3
#else
    if (Config.edg_id == "") {
        //没有边缘服务，3秒后重新登入
        delayLoginDio();
        return false;
    } 
#endif

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

    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld.ReplaceAdd("handshake_count", m_handshake_success_count);
    pld.ReplaceAdd("handshake_failed_count", m_handshake_failed_count);    
    pld.ReplaceAdd("freeheap", ESP.getFreeHeap());
    pld.ReplaceAdd("freestack", ESP.getFreeContStack());
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
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld.Add("online", 0);
    pld.Add("online_count", m_online_count + 1);

    std::string topic = cmd.expandTopic();    
#ifdef NETWORKING_V3
    topic = Config.props.mqtt_dev_status;
#endif
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
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld.ReplaceAdd("online", 1);
    pld.ReplaceAdd("online_count", m_online_count);
    pld.ReplaceAdd("timestamp", (uint64_t)millis());
    pld.ReplaceAdd("reboot", m_reboot);

#ifdef NETWORKING_V3
    cmd.topic = Config.props.mqtt_dev_status;
#endif
    cmd.send();
#ifdef NETWORKING_V3
    if (m_reboot) {
        pld.ReplaceAdd("reboot", 0);
        cmd.send();
    }
#endif
    m_reboot = 0;
};

//上线发布->DSP
void rfir::module::device::Networking::setOnlineToDsp(){
    if (Config.props.dsp_id == "")
        return;

    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="dsp";
    cmd.command.head.to.id = Config.props.dsp_id;
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = "status";
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld.Add("online", 1);
    pld.Add("online_count", m_online_count);
    cmd.send();
};

//上线发布->EDG
void rfir::module::device::Networking::setOnlineToEdg(){
    if (Config.props.edg_id == "")
        return;

    cmds::cmd::CmdMqtt cmd;
    cmd.command.head.to.type ="edg";
    cmd.command.head.to.id = Config.props.edg_id;
    cmd.command.head.entry.type ="evt";
    cmd.command.head.entry.id = "status";
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    Config.getIds(&hd);
    GDevice->getCommonProps(&pld);
    pld.Add("online", 1);
    pld.Add("online_count", m_online_count);
    cmd.send();
};

//订阅主题
void rfir::module::device::Networking::subscribe() {    
    if (!GMqttClient.mqtt.connected())
        return;

#ifdef NETWORKING_V3
    DEBUGER.printf("rfir::module::device::Networking::subscribe %s \r\n", Config.props.mqtt_dev_sub.c_str());
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_sub.c_str(), 2);
#else
    DEBUGER.printf("rfir::module::device::Networking::subscribe %s, %s \r\n",Config.props.mqtt_dsp_evt_status.c_str(), Config.props.mqtt_edg_evt_status.c_str());
    //dsp
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dsp_evt_status.c_str(), 2);

    //edg
    GMqttClient.mqtt.subscribe(Config.props.mqtt_edg_evt_status.c_str(), 2);

    //dev
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_login.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_handshake.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_get.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_set.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_reboot.c_str(), 2);
    GMqttClient.mqtt.subscribe(Config.props.mqtt_dev_svc_penet.c_str(), 2);
#endif
}

//取消订阅
void rfir::module::device::Networking::unsubscribe() {
    if (!GMqttClient.mqtt.connected())
        return;    
#ifdef NETWORKING_V3
    DEBUGER.printf("rfir::module::device::Networking::unsubscribe %s \r\n", Config.props.mqtt_dev_sub.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_sub.c_str());
#else
    DEBUGER.printf("rfir::module::device::Networking::unsubscribe \r\n");
    //dsp
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dsp_evt_status.c_str());

    //edg
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_edg_evt_status.c_str());

    //dev
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_login.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_handshake.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_get.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_set.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_reboot.c_str());
    GMqttClient.mqtt.unsubscribe(Config.props.mqtt_dev_svc_penet.c_str());
#endif
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
#ifdef NETWORKING_V3   
    delayNetworkingV3();
#else
    delayNetworking();
#endif
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
    if (cmd->topic == Config.props.mqtt_dsp_evt_status) {
        return onDsp_status_change(arg, p);
    } 
    
    if (cmd->topic == Config.props.mqtt_edg_evt_status) {
        return onEdg_status_change(arg, p);
    } 
    
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


//调度登入响应
void* rfir::module::device::Networking::onDev_login_dsp_resp(void* arg, void* p){
    auto cmd = (cmds::cmd::CmdMqtt*)p;

    DEBUGER.printf("rfir::module::device::Networking::onDev_login_dsp_resp: %s \r\n", cmd->command.pld.ToString().c_str());  
    
    std::string app_id, dom_id, dsp_id, edg_id;

    cmd->command.pld.Get("app_id", Config.props.app_id);
    cmd->command.pld.Get("dom_id", Config.props.dom_id);
    cmd->command.pld.Get("dsp_id", Config.props.dsp_id);
    cmd->command.pld.Get("edg_id", Config.props.edg_id);
    cmd->command.pld.Get("dio_id", Config.props.dio_id);
    
    
    status.logined = Config.props.app_id != "" && Config.props.dom_id != "" && Config.props.dsp_id !="" && Config.props.edg_id != "";

    if (status.logined) {
        unsubscribe();
        Config.fixup();
        subscribe();
        handshake();
    } else {
        delayLoginDio();
    }

    return 0;
};

//调度登入超时
void* rfir::module::device::Networking::onDev_login_dsp_timeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onDev_login_dsp_timeout");
    status.logined = Config.props.app_id != "" && Config.props.dom_id != "" && Config.props.dsp_id !="" && Config.props.edg_id != "";

    if (status.logined) {
        handshake();        
    } else {
        loginDio();
    }

    return 0;
};



//数据登入响应
void* rfir::module::device::Networking::onDev_login_dio_resp(void* arg, void* p){
    GEventTimer.remove(m_login_reset_handler);
    m_login_reset_handler = 0;
    auto cmd = (cmds::cmd::CmdMqtt*)p;

    DEBUGER.printf("rfir::module::device::Networking::onDev_login_dio_resp: %s \r\n", cmd->command.pld.ToString().c_str());  
    
    std::string app_id, dom_id, dsp_id, edg_id, dio_id;

    cmd->command.pld.Get("app_id", Config.props.app_id);
    cmd->command.pld.Get("dom_id", Config.props.dom_id);
    cmd->command.pld.Get("dsp_id", Config.props.dsp_id);
    cmd->command.pld.Get("dio_id", Config.props.dio_id);
    cmd->command.pld.Get("edg_id", Config.props.edg_id);

    unsubscribe();   
    Config.fixup();
    subscribe();
    status.logined = false;
    // GMqttClient.delayDisconnectToMqtt(100);
    this->loginDsp();
    return (void*)1;    
};

//数据登入超时
void* rfir::module::device::Networking::onDev_login_dio_timeout(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onDev_login_dio_timeout");
    loginDio();
    return 0;
};




//边缘握手响应
void* rfir::module::device::Networking::onDev_handshake_resp(void* arg, void* p){
    GLed.stop();
    status.logined = true;
    status.handshaked = true;
#ifdef NETWORKING_V3
#else
    setOnlineToEdg();
#endif
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
#ifdef NETWORKING_V3
    delayHandshake(Config.props.networking_re_handshake_timeout);
#else
    loginDio();
#endif
    
    return 0;
};



//调度服务状态通知
void* rfir::module::device::Networking::onDsp_status_change(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onDsp_status_change");
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    if (cmd->command.head.stp == 0) {
        setOnlineToDsp();
        setOnline();
    }
    return 0;
};

//边缘服务状态通知
void* rfir::module::device::Networking::onEdg_status_change(void* arg, void* p){
    DEBUGER.println("rfir::module::device::Networking::onEdg_status_change");
    auto cmd = (cmds::cmd::CmdMqtt*)p;
    if (cmd->command.head.stp == 0) {
        setOnlineToEdg();
        //握手边缘
        handshake();
    }
    return 0;
};



void rfir::module::device::Networking::delayNetworkingV3(int delay_ms){
    GEventTimer.delay(delay_ms, std::bind(&Networking::doNetworkingV3, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
};

void* rfir::module::device::Networking::doNetworkingV3(void* arg, void* p){    
    unsubscribe();
    subscribe();
    m_online_count++;
    setOnline();
 
    return 0;
};

rfir::module::device::Networking GNetworking;
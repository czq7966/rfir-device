#include "networking.h"
#include "network/module/mqtt/async-client.h"
#include "cmds/cmd/cmd.h"
#include "cmds/cmd/reg-table.h"
#include "rfir/util/event-timer.h"
#include "service/config.h"

//事件注册
void service::Networking::start(){
    GMqttClient.events.onMqttMessage.add(this, [](void* arg, void* p) -> void* {
        auto msg = (::network::module::mqtt::AClient::Message*)p;
        if (msg->total < sizeof(cmds::cmd::Cmd::Head) ) {
            GDebuger.print("GMqttClient.events.onMqttMessage total is low, topic: ");
            GDebuger.print(msg->topic);
            GDebuger.print(" total:");
            GDebuger.print(msg->total);
            return 0;
        }

        GRecvCmd.recv(msg->payload, msg->len);
        return 0;
    }); 

    GConfig.events.ready.add(this,  [this](void* arg, void* p)-> void*{
        setWill();
        return 0;
    });

    GMqttClient.events.onMqttConnect.add(this, [this](void* arg, void* p)-> void*{
        subscribe();
        setOnline();
        handshake();
        return 0;
    });
    GMqttClient.events.onMqttDisconnect.add(this, [this](void* arg, void* p)-> void*{
        uint16_t dev_offline_count = GRegTable.tables.get(GRegTable.keys.dev_offline_count);
        dev_offline_count++;
        GRegTable.tables.add(GRegTable.keys.dev_offline_count, dev_offline_count);
        return 0;
    });
};


void service::Networking::loop(){

}


void service::Networking::setWill(){
    cmds::cmd::Cmd::reset(&this->willPayload);
    this->willPayload.cmd_id = cmds::cmd::CmdId::offline;
    this->setWill(GRegTable.values.mqtt_pub_topic, (const char*)&willPayload, sizeof(willPayload), 2, true);
};

void service::Networking::setOnline(){
    std::list<int> ids;
    ids.push_back(GRegTable.keys.dev_offline_count);
    ids.push_back(GRegTable.keys.ota_version);
    GSendCmd.reset();
    GSendCmd.head->cmd_id = cmds::cmd::CmdId::online;
    GSendCmd.send(ids);

};

void service::Networking::subscribe(){
    GMqttClient.mqtt.subscribe(GRegTable.values.mqtt_sub_topic, 2);    
};


bool service::Networking::handshake(cmds::cmd::Cmd* cmd){
    if (GMqttClient.mqtt.connected()) {
        GDebuger.println(F("service::Networking::handshake"));

        std::list<int> ids;
        ids.push_back(GRegTable.keys.net_handshake_count);
        ids.push_back(GRegTable.keys.dev_offline_count);

        GSendCmd.reset();
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::handshake;
        if (cmd){
            GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
            GSendCmd.head->cmd_stp = 1;
        }
        GSendCmd.send(ids);              
    }

    delayHandshake(GRegTable.tables.get(GRegTable.keys.net_handshake_timeout) * 1000);
    return true;
};

void service::Networking::delayHandshake(int delay_ms){
    GEventTimer.remove(m_handshake_andler);
    m_handshake_andler = 
    GEventTimer.delay(delay_ms, [this](void*, void*)->void*{       
        handshake();
        return 0;
    });

};

int service::Networking::publish(const char* topic, const char* payload , size_t length, uint8_t qos , bool retain, bool dup, uint16_t message_id){
    if (GMqttClient.mqtt.connected())
        return GMqttClient.mqtt.publish(topic, qos, retain, payload, length, dup, message_id);            

    return 0;
};

int service::Networking::setWill(const char* topic, const char* payload , size_t length, uint8_t qos , bool retain){
    GMqttClient.mqtt.setWill(topic, qos, retain, payload, length);
    return 0;
};

service::Networking GNetworking;
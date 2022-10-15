#include "networking.h"
#include "network/module/mqtt/async-client.h"
#include "cmds/cmd/cmd.h"
#include "cmds/cmd/reg-table.h"
#include "rfir/util/event-timer.h"

//事件注册
void service::Networking::start(){
    setWill();

    GMqttClient.events.onMqttConnect.add(this, [this](void* arg, void* p)-> void*{
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
    GMqttClient.events.onMqttMessage.add(this, [this](void* arg, void* p)-> void*{
        auto msg = (::network::module::mqtt::AClient::Message*)p;
        if (msg->total < (sizeof(cmds::cmd::Cmd::Head) - sizeof(char*)) )
            return 0;

        GRecvCmd.recv(msg->payload, msg->len);

        return 0;
    });

    GRecvCmd.events.recv.add(this, [this](void* arg, void* p)-> void*{
        return 0;
    });

    GSendCmd.events.send.add(this, [this](void* arg, void* p)-> void*{

        return 0;
    });

};


void service::Networking::loop(){

}


void service::Networking::setWill(){
    this->willPayload.cmd_id = cmds::cmd::CmdId::offline;
    GMqttClient.mqtt.setWill(GRegTable.values.mqtt_pub_topic, 2, true, (const char*)&willPayload, sizeof(willPayload));
};

void service::Networking::setOnline(){
    std::list<int> ids;
    ids.push_back(GRegTable.keys.dev_offline_count);
    GSendCmd.reset();
    GSendCmd.head->cmd_id = cmds::cmd::CmdId::online;
    GSendCmd.send();

};

void service::Networking::subscribe(){
    GMqttClient.mqtt.subscribe(GRegTable.values.mqtt_sub_topic, 2);
};


bool service::Networking::handshake(){
    if (GMqttClient.mqtt.connected()) {
        uint16_t value = GRegTable.tables.get(GRegTable.keys.net_handshake_count);
        value++;
        GRegTable.tables.add(GRegTable.keys.net_handshake_count, value);

        std::list<int> ids;
        ids.push_back(GRegTable.keys.net_handshake_count);
        ids.push_back(GRegTable.keys.dev_offline_count);

        GSendCmd.reset();
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::handshake;
        GSendCmd.send();
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

service::Networking GNetworking;
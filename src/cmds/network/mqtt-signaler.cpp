
#include "mqtt-signaler.h"
#include "network/service/mqtt/client.h"

cmds::network::MqttSignaler::MqttSignaler(::network::module::mqtt::Client* p) {
    setMqtt(0);
    setMqtt(p);
};

void cmds::network::MqttSignaler::setMqtt(::network::module::mqtt::Client* p){
    if (mqtt) {
        mqtt->events.onConnect.remove((void*)this);
        mqtt->events.onMessage.remove((void*)this);    
    }

    mqtt = p;    

    if (mqtt) {
        mqtt->events.onConnect.add((void*)this, OnConnect, (void*)this);
        mqtt->events.onMessage.add((void*)this, OnMessage, (void*)this);            
    }

};

int cmds::network::MqttSignaler::write(void* p) {
    DEBUGER.println("cmds::network::MqttSignaler::write");
    return write((cmds::cmd::CmdMqtt*)p);
};
int cmds::network::MqttSignaler::write(cmds::cmd::CmdMqtt* cmd) {
    DEBUGER.println("cmds::network::MqttSignaler::write2");
    if (mqtt && cmd->topic.length() > 0) {
        DEBUGER.println("cmds::network::MqttSignaler::write3");
        return mqtt->publish(cmd->topic.c_str(), cmd->command.toString().c_str());
    }

    return 0;
};


cmds::network::MqttSignaler GMqttSignaler(0);
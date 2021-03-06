
#include "mqtt-signaler.h"


void cmds::network::MqttSignaler::setMqtt(::network::module::mqtt::AClient* p){
    if (mqtt) {
        mqtt->events.onMqttConnect.remove((void*)this);
        mqtt->events.onMqttDisconnect.remove((void*)this);
        mqtt->events.onMqttMessage.remove((void*)this);    
    }

    mqtt = p;    

    if (mqtt) {
        mqtt->events.onMqttConnect.add((void*)this, std::bind(&MqttSignaler::onConnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
        mqtt->events.onMqttDisconnect.add((void*)this, std::bind(&MqttSignaler::onDisconnect, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
        mqtt->events.onMqttMessage.add((void*)this, std::bind(&MqttSignaler::onMessage, this, std::placeholders::_1, std::placeholders::_2), (void*)this);            
    }

};

int cmds::network::MqttSignaler::write(void* p) {
    return write((cmds::cmd::CmdMqtt*)p);
};
int cmds::network::MqttSignaler::write(cmds::cmd::CmdMqtt* cmd) {
    if (mqtt) {
        std::string topic = cmd->topic;
        if (topic == "") {
            topic = cmd->expandTopic();
        }
        DEBUGER.printf("cmds::network::MqttSignaler::write %s\r\n", topic.c_str());
        return mqtt->publish(topic.c_str(), cmd->command.toString().c_str(), cmd->command.head.retain, cmd->command.head.qos);
    }

    return 0;
};


cmds::network::MqttSignaler GMqttSignaler;
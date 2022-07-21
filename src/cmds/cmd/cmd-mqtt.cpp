#include "cmd-mqtt.h"
#include "cmd-dispatcher.h"

std::string cmds::cmd::CmdMqtt::topicPrefix = "";

std::string cmds::cmd::CmdMqtt::ZeroTopic(std::string topic){
    if (topic.length() == 0)
        return "0";
    return topic;
};

std::string cmds::cmd::CmdMqtt::expandTopic(){
#ifdef NETWORKING_V3   
    return Config.mqtt_dev_pub;
#else
    return  (prefix == "" ? topicPrefix : prefix) +
            ZeroTopic(command.head.from.type) + "/" + 
            ZeroTopic(command.head.from.id) + "/" + 
            ZeroTopic(command.head.to.type) + "/" + 
            ZeroTopic(command.head.to.id) + "/" + 
            ZeroTopic(command.head.entry.type) + "/" + 
            ZeroTopic(command.head.entry.id);
#endif
};

bool cmds::cmd::CmdMqtt::send(const void* p) {
    DEBUGER.println("cmds::cmd::CmdMqtt::send");
    return GCmdDispatcher.sendCmd(this);
};


#include "cmd-mqtt.h"
#include "cmd-dispatcher.h"


std::string cmds::cmd::CmdMqtt::zeroTopic(std::string topic){
    if (topic.length() == 0)
        return "0";
    return topic;
};

std::string cmds::cmd::CmdMqtt::expandTopic(){
    return  zeroTopic(command.head.from.type) + "/" + 
            zeroTopic(command.head.from.id) + "/" + 
            zeroTopic(command.head.to.type) + "/" + 
            zeroTopic(command.head.to.id) + "/" + 
            zeroTopic(command.head.entry.type) + "/" + 
            zeroTopic(command.head.entry.id);
};

bool cmds::cmd::CmdMqtt::send(const void* p) {
    DEBUGER.println("cmds::cmd::CmdMqtt::send");
    return GCmdDispatcher->sendCmd(this);
};


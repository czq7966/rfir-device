#include "cmd-mqtt.h"
#include "cmd-dispatcher.h"

bool cmds::cmd::CmdMqtt::send(const void* p) {
    DEBUGER.println("cmds::cmd::CmdMqtt::send");
    return GCmdDispatcher->sendCmd(this);
};




void* cmds::cmd::CmdMqtt::clone() {
    auto cmd = new CmdMqtt();
    cmd->topic = this->topic;
    this->cloneTo(*cmd);
    return (void*)cmd;
};
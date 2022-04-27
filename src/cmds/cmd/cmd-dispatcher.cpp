#include "cmd-dispatcher.h"
#include "../network/mqtt-signaler.h"
#include "rfir/util/event-timer.h"

cmds::cmd::CmdDispatcher::CmdDispatcher(cmds::network::Signaler* p){
    signaler = p;
    signaler->events.onConnect.add((void*)this, OnConnect, (void*)this);
    signaler->events.onDisconnect.add((void*)this, OnDisconnect, (void*)this);
    signaler->events.onMessage.add((void*)this, OnMessage, (void*)this);    

};
cmds::cmd::CmdDispatcher::~CmdDispatcher(){
    signaler->events.onConnect.remove((void*)this);
    signaler->events.onDisconnect.remove((void*)this);
    signaler->events.onMessage.remove((void*)this);        
    this->signaler = 0;
};

void* cmds::cmd::CmdDispatcher::OnConnect(void* arg, void* p){
    DEBUGER.println("cmds::cmd::CmdDispatcher::OnConnect");
    auto dispatcher = (CmdDispatcher*)arg;
    return dispatcher->events.onConnect.emit(p);
    return 0;
};
void* cmds::cmd::CmdDispatcher::OnDisconnect(void* arg, void* p){
    auto dispatcher = (CmdDispatcher*)arg;
    return dispatcher->events.onConnect.emit(p);    
};
void* cmds::cmd::CmdDispatcher::OnMessage(void* arg, void* p){
    auto dispatcher = (CmdDispatcher*)arg;    
    return dispatcher->events.onMessage.emit(p);    
};   

void* cmds::cmd::CmdDispatcher::OnResp(void* arg, void* p) {
    DEBUGER.print("cmds::cmd::CmdDispatcher::OnResp: ");
    auto cmd = (cmds::cmd::CmdBase*)p;
    auto dispatcher = (CmdDispatcher*)arg;  
    if (cmd->command.isRespCmd()) {
        auto sid = cmd->command.getSid();
        DEBUGER.println(String(sid));
        if (sid > 0) {
            cmds::cmd::CmdBase::Events* events = 0;
            if (dispatcher->wait_resp_queue.remove(sid, events)) {
                if (cmd->events.onResp.callback) {
                    cmd->events.onResp.callback(cmd->events.onResp.cbArg, p);
                }
                delete events;
                return (void*)1;
            }
        }
    }

    return 0;
}

void* cmds::cmd::CmdDispatcher::OnRespTimeout(void* arg, void* p) {
    DEBUGER.println("cmds::cmd::CmdDispatcher::OnRespTimeout");
    auto sid = (uint32_t)p;
    auto dispatcher = (CmdDispatcher*)arg;
    cmds::cmd::CmdBase::Events* events = 0;
    if (!dispatcher) {
        DEBUGER.println("not dispatcher");
    }
    dispatcher->wait_resp_queue.remove(sid, events);
    if (events && events->onTimeout.callback) {
        events->onTimeout.callback(events->onTimeout.cbArg, p);
    }

    delete events;
    
    return 0;
}

bool cmds::cmd::CmdDispatcher::sendCmd(cmds::cmd::CmdBase* cmd){    
    DEBUGER.println("cmds::cmd::CmdDispatcher::sendCmd");
    auto result = this->signaler->write((void*)cmd);
    if (cmd->command.isNeedResp()) {
        DEBUGER.println("cmd->command.isNeedResp()");
        auto events = cmd->events.clone();
        wait_resp_queue.add(cmd->command.getSid(), events);
        GEventTimer.delay(cmd->respTimeout, OnRespTimeout, (void*)this, cmd->command.getSid());
    } else {
        DEBUGER.println("not cmd->command.isNeedResp()");

    }
    return result;
};


//MqttDispatcher

cmds::cmd::MqttDispatcher::MqttDispatcher(cmds::network::MqttSignaler* p): CmdDispatcher(p) {
    events.onMessage.add((void*)&OnMqttMessage, OnMqttMessage, (void*)this);
};
cmds::cmd::MqttDispatcher::~MqttDispatcher(){    
    events.onMessage.add((void*)&OnMqttMessage, OnMqttMessage, (void*)this);
};

void* cmds::cmd::MqttDispatcher::OnMqttMessage(void* arg, void* p){
    DEBUGER.println("cmds::cmd::MqttDispatcher::OnMqttMessage");
    auto msg = (::network::module::mqtt::Client::Message*)p;
    auto dispatcher = (CmdDispatcher*)arg;   
    cmds::cmd::CmdMqtt cmd;    
    cmd.topic = msg->topic;
    cmd.command.fromString(msg->bytes);
    if (cmd.command.isRespCmd() && OnResp(arg, &cmd) ) {
        return 0;
    }
     
    return dispatcher->events.onCommand.emit(&cmd); 
}; 


cmds::cmd::CmdDispatcher* GCmdDispatcher = 0; 
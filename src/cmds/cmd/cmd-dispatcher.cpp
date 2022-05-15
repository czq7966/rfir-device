#include "cmd-dispatcher.h"
#include "../network/mqtt-signaler.h"
#include "rfir/util/event-timer.h"

void cmds::cmd::CmdDispatcher::setSignaler(cmds::network::Signaler* p){
    if (signaler) {
        signaler->events.onConnect.remove((void*)this);
        signaler->events.onDisconnect.remove((void*)this);
        signaler->events.onMessage.remove((void*)this);    
    }

    signaler = p;
    if (signaler) {
        signaler->events.onConnect.add((void*)this, std::bind(&CmdDispatcher::onConnect, this, std::placeholders::_1,  std::placeholders::_2), (void*)this);
        signaler->events.onDisconnect.add((void*)this, std::bind(&CmdDispatcher::onDisconnect, this, std::placeholders::_1,  std::placeholders::_2), (void*)this);
        signaler->events.onMessage.add((void*)this, std::bind(&CmdDispatcher::onMessage, this, std::placeholders::_1,  std::placeholders::_2), (void*)this);    
    }
  
};

void* cmds::cmd::CmdDispatcher::onConnect(void* arg, void* p){
    DEBUGER.println("cmds::cmd::CmdDispatcher::OnConnect");
    return this->events.onConnect.emit(p);
    // auto dispatcher = (CmdDispatcher*)arg;
    // return dispatcher->events.onConnect.emit(p);
    // return 0;
};
void* cmds::cmd::CmdDispatcher::onDisconnect(void* arg, void* p){
    return this->events.onDisconnect.emit(p);
    // auto dispatcher = (CmdDispatcher*)arg;
    // return dispatcher->events.onConnect.emit(p);    
};
void* cmds::cmd::CmdDispatcher::onMessage(void* arg, void* p){
    return this->events.onMessage.emit(p);

    // auto dispatcher = (CmdDispatcher*)arg;    
    // return dispatcher->events.onMessage.emit(p);    
};   

void* cmds::cmd::CmdDispatcher::onResp(void* arg, void* p) {
    DEBUGER.print("cmds::cmd::CmdDispatcher::OnResp: ");    
    auto cmd = (cmds::cmd::CmdBase*)p;
    if (cmd->command.isRespCmd()) {
        auto sid = cmd->command.getSid();
        DEBUGER.println(String(sid));
        if (sid > 0) {
            cmds::cmd::CmdBase::Events events;
            GEventTimer.remove(sid);
            this->wait_resp_queue.remove(sid, events);
            if (events.onResp.callback) {
                return events.onResp.callback(events.onResp.cbArg, p);
            }
        }
    }

    return 0;

    // DEBUGER.print("cmds::cmd::CmdDispatcher::OnResp: ");    
    // auto cmd = (cmds::cmd::CmdBase*)p;
    // auto dispatcher = (CmdDispatcher*)arg;  
    // if (cmd->command.isRespCmd()) {
    //     auto sid = cmd->command.getSid();
    //     DEBUGER.println(String(sid));
    //     if (sid > 0) {
    //         cmds::cmd::CmdBase::Events* events = 0;
    //         GEventTimer.remove(sid);
    //         dispatcher->wait_resp_queue.remove(sid, events);
    //         if (events && events->onResp.callback) {
    //             events->onResp.callback(events->onResp.cbArg, p);
    //         }
    //         delete events;
    //     }
    // }

    // return 0;
}

void* cmds::cmd::CmdDispatcher::onRespTimeout(void* arg, void* p) {
    DEBUGER.println("cmds::cmd::CmdDispatcher::onRespTimeout");
    auto sid = (uint32_t)p;

    cmds::cmd::CmdBase::Events events;
    this->wait_resp_queue.remove(sid, events);
    if (events.onTimeout.callback) {
        return events.onTimeout.callback(events.onTimeout.cbArg, p);
    }
  
    return 0;
}

bool cmds::cmd::CmdDispatcher::sendCmd(cmds::cmd::CmdBase* cmd){    
    DEBUGER.println("cmds::cmd::CmdDispatcher::sendCmd");  
    if (!signaler)
        return false;
          
    auto result = this->signaler->write((void*)cmd);
    if (cmd->command.isNeedResp()) {
        auto events = cmd->events;
        wait_resp_queue.add(cmd->command.getSid(), events);
        GEventTimer.delay(cmd->respTimeout, std::bind(&CmdDispatcher::onRespTimeout, this, std::placeholders::_1, std::placeholders::_2), (void*)this, cmd->command.getSid());
    } 
    return result;
};

bool cmds::cmd::CmdDispatcher::removeWaitResp(uint32_t sid){
    GEventTimer.remove(sid);
    return wait_resp_queue.remove(sid);
};

//MqttDispatcher

void cmds::cmd::MqttDispatcher::setSignaler(cmds::network::Signaler* p){
    events.onMessage.remove((void*)&MqttDispatcher::onMqttMessage);
    CmdDispatcher::setSignaler(p);
    events.onMessage.add((void*)&MqttDispatcher::onMqttMessage, std::bind(&MqttDispatcher::onMqttMessage, this, std::placeholders::_1, std::placeholders::_2), (void*)this);
}
void* cmds::cmd::MqttDispatcher::onMqttMessage(void* arg, void* p){
    DEBUGER.print("cmds::cmd::MqttDispatcher::OnMqttMessage: ");
    auto msg = (::network::module::mqtt::AClient::Message*)p;
    DEBUGER.println(msg->topic);
    if(msg->total > msg->len) {
        DEBUGER.println("Mqtt message max length: 1460");
        return 0;
    }
    
    cmds::cmd::CmdMqtt cmd;    
    cmd.topic = msg->topic;
    if (msg->payload)
        cmd.command.fromString(msg->payload);
    if (cmd.command.isRespCmd() && onResp(arg, &cmd) ) {
        return 0;
    }
     
    return this->events.onCommand.emit(&cmd); 
}; 


cmds::cmd::MqttDispatcher GCmdDispatcher;
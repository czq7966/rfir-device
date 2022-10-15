#include "cmds.h"
#include "cmds/cmd/cmd.h"
#include "cmds/cmd/reg-table.h"
#include "network/module/mqtt/async-client.h"


void service::Cmds::start(){
    GMqttClient.events.onMqttMessage.add(0, [](void* arg, void* p) -> void* {
        auto msg = (::network::module::mqtt::AClient::Message*)p;
        if (msg->total < sizeof(cmds::cmd::Cmd::Head) )
            return 0;

        GRecvCmd.recv(msg->payload, msg->len);
        return 0;
    });  
    GRecvCmd.events.recv.add(this, [this](void* arg, void* p)->void*{
        this->onCmd(&GRecvCmd);
        return 0;
    });
    
    GSendCmd.events.send.add(this, [this](void* arg, void* p)->void*{
        if (GMqttClient.mqtt.connected())
            GMqttClient.mqtt.publish(GRegTable.values.mqtt_pub_topic, 2, true, GSendCmd.params.buf, int(p));            
        
        return 0;
    });    

};

void service::Cmds::loop(){

};

void service::Cmds::onCmd(cmds::cmd::RecvCmd* cmd){
    switch (cmd->head->cmd_id)
    {
    case cmds::cmd::CmdId::config :
        
        break;
    case cmds::cmd::CmdId::handshake :
        
        break;
    case cmds::cmd::CmdId::reboot :
        
        break;
    case cmds::cmd::CmdId::get :
        
        break;
    case cmds::cmd::CmdId::set :
        
        break;
    case cmds::cmd::CmdId::report :
        
        break;
    case cmds::cmd::CmdId::penet :
        
        break;
    case cmds::cmd::CmdId::update :
        
        break;

    default:
        break;
    }

};

service::Cmds GCmds;     

               

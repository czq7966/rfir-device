#include "cmds.h"
#include "cmds/cmd/cmd.h"
#include "cmds/cmd/reg-table.h"
#include "network/module/mqtt/async-client.h"
#include "network/module/ota/updater.h"
#include "rfir/util/util.h"
#include "rfir/util/event-timer.h"
#include "networking.h"
#include "device.h"
#include "config.h"


void service::Cmds::start(){
    GMqttClient.events.onMqttMessage.add(0, [](void* arg, void* p) -> void* {
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
    GRecvCmd.events.recv.add(this, [this](void* arg, void* p)->void*{
        this->onCmd(&GRecvCmd);
        return 0;
    });
    
    GSendCmd.events.send.add(this, [this](void* arg, void* p)->void*{
        if (GMqttClient.mqtt.connected())
            GMqttClient.mqtt.publish(GRegTable.values.mqtt_pub_topic, 2, true, (const char*)GSendCmd.head, int(p));            
        
        return 0;
    });    

};

void service::Cmds::loop(){

};

void service::Cmds::onCmd(cmds::cmd::RecvCmd* cmd){
    GDebuger.print("service::Cmds::onCmd: ");
    GDebuger.println(cmd->head->cmd_id);
    switch (cmd->head->cmd_id)
    {
        case cmds::cmd::CmdId::config :
            this->onCmd_config(cmd);            
            break;
        case cmds::cmd::CmdId::handshake :
            this->onCmd_handshake(cmd);
            break;
        case cmds::cmd::CmdId::reboot :
            this->onCmd_reboot(cmd);            
            break;
        case cmds::cmd::CmdId::get :
            this->onCmd_get(cmd);            
            break;
        case cmds::cmd::CmdId::set :
            this->onCmd_set(cmd);            
            break;
        case cmds::cmd::CmdId::report :
            this->onCmd_report(cmd);            
            break;
        case cmds::cmd::CmdId::penet :
            this->onCmd_penet(cmd);            
            break;
        case cmds::cmd::CmdId::update :
            this->onCmd_update(cmd);            
            break;
        case cmds::cmd::CmdId::online :
           
            break;
        case cmds::cmd::CmdId::offline :

            break;
        case cmds::cmd::CmdId::resetconfig :
            this->onCmd_reset_config(cmd);            
            break;
        case cmds::cmd::CmdId::device_joined :
            this->onCmd_device_joined(cmd);            
            break;
        case cmds::cmd::CmdId::device_leave :
            this->onCmd_device_leave(cmd);            
            break;
        case cmds::cmd::CmdId::device_interview :
            this->onCmd_device_interview(cmd);            
            break;                        
        default:
            break;
    }

};


void service::Cmds::onCmd_config(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    if (GDevice->onCmd_config(cmd) != -1){
        std::list<int> ids;
        GRegTable.merge(&cmd->regTable, ids);

        for (auto it = ids.begin(); it != ids.end(); it++)
        {
            GConfig.saved.add(*it, 0);
        }        

        GConfig.save();  

        //Debug
        GDebuger.enabled =  GRegTable.tables.get(GRegTable.keys.serial_debug);

        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::config;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        GSendCmd.send(ids);
    };
};

void service::Cmds::onCmd_handshake(cmds::cmd::RecvCmd* cmd){    
    if (GDevice->onCmd_handshake(cmd) != -1 ){
        if (cmd->head->cmd_stp == 0)
            GNetworking.handshake(0);
        else {
            uint16_t value = GRegTable.tables.get(GRegTable.keys.net_handshake_count);
            value++;
            GRegTable.tables.add(GRegTable.keys.net_handshake_count, value);  
        }
    };    
};

void service::Cmds::onCmd_reboot(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_reboot(cmd) != -1){
        rfir::util::Util::Reset();
    };    
};

void service::Cmds::onCmd_get(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    if (GDevice->onCmd_get(cmd) != -1){
        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::get;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        std::list<int> ids;
        cmd->regTable.tables.getKeys(ids);
        GSendCmd.send(ids);
    };
};

void service::Cmds::onCmd_set(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    if (GDevice->onCmd_set(cmd) != -1){
        std::list<int> ids;
        GRegTable.merge(&cmd->regTable, ids);

        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::get;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        ids.clear();
        cmd->regTable.tables.getKeys(ids);
        GSendCmd.send(ids);
    };
};

void service::Cmds::onCmd_report(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_report(cmd) != -1){

    };
};

void service::Cmds::onCmd_penet(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_penet(cmd) != -1){

    };
};

void service::Cmds::onCmd_update(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_update(cmd) != -1){
        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::update;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        std::list<int> ids;
        ids.push_back(GRegTable.keys.ota_version);
        GSendCmd.send(ids);
        GEventTimer.delay(500, [this](void*, void*)->void*{
            GOTAUpdater.doCheckAndUpdate(0, 0);
            return 0;
        });        
    };
    
};

void service::Cmds::onCmd_reset_config(cmds::cmd::RecvCmd* cmd){
    cmd->regTable.dump();
    if (GDevice->onCmd_reset_config(cmd) != -1){
        if (cmd->regTable.tables.getSize() == 0) {
            GConfig.resetConfig(true);
        }
        else {
            auto map = cmd->regTable.tables.getMap();
            for (auto  it = map->begin(); it != map->end(); it++)
            {
                GConfig.saved.remove(it->first);
            }
            
            GConfig.save();            
            rfir::util::Util::Reset(100);
        }
    };    
};

void service::Cmds::onCmd_device_joined(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_device_joined(cmd) != -1){

    };
};

void service::Cmds::onCmd_device_leave(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_device_leave(cmd) != -1){

    };
};

void service::Cmds::onCmd_device_interview(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_device_interview(cmd) != -1){

    };
};

service::Cmds GCmds;     

               

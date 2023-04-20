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
    GRecvCmd.events.recv.add(this, [this](void* arg, void* p)->void*{
        this->onCmd(&GRecvCmd);
        return 0;
    });
    
    GSendCmd.events.send.add(this, [this](void* arg, void* p)->void*{
        GNetworking.publish(GRegTable.values.mqtt_pub_topic, (const char*)GSendCmd.head, int(p));
        return 0;
    });    

    //Report 
    GMqttClient.events.onMqttConnect.add(this, [this](void* arg, void* p)-> void*{
        delay_report_reg(1000);
        delay_report_penet(1000);
        return 0;
    });

    //Report 
    LMqttClient.events.onMqttConnect.add(this, [this](void* arg, void* p)-> void*{
        delay_report_reg(1000);
        delay_report_penet(1000);
        return 0;
    });

    //定时重启检查
    delay_reboot_interval(1000 * 60);

};

void service::Cmds::loop(){

};

void service::Cmds::delay_report_reg(int timeout_ms){
    GEventTimer.remove(this->delay_report_reg_handler);
    this->delay_report_reg_handler = GEventTimer.delay(timeout_ms, [this](void* arg, void* p)-> void*{
        do_report_reg();
        delay_report_reg(GRegTable.tables.get(GRegTable.keys.report_reg_timeout) * 1000);
        return 0;
    });    
};

void service::Cmds::delay_report_penet(int timeout_ms){
    GEventTimer.remove(this->delay_report_penet_handler);
    this->delay_report_penet_handler = GEventTimer.delay(timeout_ms, [this](void* arg, void* p)-> void*{
        do_report_penet();
        delay_report_penet(GRegTable.tables.get(GRegTable.keys.report_penet_timeout) * 1000);
        return 0;
    });   
};

void service::Cmds::delay_reboot_interval(int timeout_ms){
    GEventTimer.remove(this->delay_reboot_interval_handler);
    this->delay_reboot_interval_handler = GEventTimer.delay(timeout_ms, [this](void* arg, void* p)-> void*{
        do_reboot_interval();
        delay_reboot_interval(60 * 1000);
        return 0;
    });    
};

void service::Cmds::do_report_reg(){
    if (GRegTable.get(GRegTable.keys.report_reg_enable)) {
        GDebuger.println(F("service::Cmds::do_report_reg "));

        auto data = GRegTable.decodeVectorAddress(GRegTable.get(GRegTable.keys.report_reg_data));
        if (data && data->size() > 0 &&  (data->size() % 4 == 0)) { //Key+Value列表
            cmds::cmd::RecvCmd* cmd = &GRecvCmd;
            cmds::cmd::Cmd::Head hd;
            cmd->head = &hd;
            cmds::cmd::Cmd::reset(cmd->head);
            cmd->regTable.tables.clear();

            int i = 0;
            while (i < data->size())
            {
                int key = (*data)[i++] + ( (*data)[i++] << 8 );
                int value = (*data)[i++] + ( (*data)[i++] << 8 );
                cmd->regTable.tables.add(key, value);
            }
                   
            cmd->head->cmd_id = cmds::cmd::CmdId::report;
            onCmd_get(cmd);  
        }
    }

};

void service::Cmds::do_report_penet(){
    if (GRegTable.get(GRegTable.keys.report_penet_enable)) {
        GDebuger.println(F("service::Cmds::do_report_penet "));

        auto data = GRegTable.decodeVectorAddress(GRegTable.get(GRegTable.keys.report_penet_data));
        if (data ) { //长度+内容  .....
            int offset = 0;
            int idx = 0;

            while(offset + 2 < data->size()){
                int size = (*data)[offset] + ((*data)[offset + 1] << 8);
                GEventTimer.delay(200 * idx, [this](void* arg, void* p)-> void*{
                    do_report_penet_offset((int)arg);
                    return 0;
                }, (void*)offset);   
                offset = offset + 2 + size;
                idx++;
            }
        }
    }
};

void service::Cmds::do_report_penet_offset(int offset) {
    auto data = GRegTable.decodeVectorAddress(GRegTable.get(GRegTable.keys.report_penet_data));
    if (data && data->size() > offset + 2 ) { //长度+内容  .....
        int idx = offset;
        int size = (*data)[idx++] + ((*data)[idx++] << 8);

        if (size + idx <= data->size() ) {
            cmds::cmd::RecvCmd* cmd = &GRecvCmd;
            cmds::cmd::Cmd::Head hd;
            cmd->head = &hd;
            cmds::cmd::Cmd::reset(cmd->head);
            cmd->head->cmd_id = cmds::cmd::CmdId::penet;
            cmd->regTable.tables.add(GRegTable.keys.penet_data, GRegTable.encodeVectorAddress(data));
            cmd->regTable.sizes.add(GRegTable.keys.penet_data, size);
            GDevice->onCmd_penet(cmd, idx);
        }
    }
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
        case cmds::cmd::CmdId::get_gpio :
            this->onCmd_get_gpio(cmd);            
            break;
        case cmds::cmd::CmdId::set_gpio :
            this->onCmd_set_gpio(cmd);            
            break;
        case cmds::cmd::CmdId::rfir_send :
            this->onCmd_rfir_send(cmd);            
            break;
        case cmds::cmd::CmdId::rfir_sniff :
            this->onCmd_rfir_sniff(cmd);            
            break;   
        case cmds::cmd::CmdId::intranet :
            this->onCmd_intranet(cmd);            
            break;                    
        default:
            break;
    }

};

void service::Cmds::do_reboot_interval(){
    if (GRegTable.get(GRegTable.keys.reboot_interval_enable)) {
        GDebuger.println(F("service::Cmds::do_reboot_interval "));
        int timeoutMin = GRegTable.tables.get(GRegTable.keys.report_reg_timeout);
        if (timeoutMin < 30){
            timeoutMin = 30;
        }

        if (millis() > timeoutMin * 60 * 1000) {
            rfir::util::Util::Reset();
        }

    }

};

void service::Cmds::onCmd_config(cmds::cmd::RecvCmd* cmd){
    if (GDevice->onCmd_config(cmd) != -1){
        std::list<int> ids;
        GRegTable.merge(&cmd->regTable, ids);

        for (auto it = ids.begin(); it != ids.end(); it++)
        {
            GConfig.saved.add(*it, 0);
        }        
        //
        GConfig.save();  

        //Debug
        GDebuger.enabled =  GRegTable.tables.get(GRegTable.keys.serial_debug);

        //Resp
        GSendCmd.reset();
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

        //get_gpio
        if (cmd->regTable.tables.exist(GRegTable.keys.gpio_rw_pin)) {
            int pin = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_pin);
            int value = rfir::util::Util::GetGpio(pin);
            GRegTable.tables.add(GRegTable.keys.gpio_rw_pin, pin);
            GRegTable.tables.add(GRegTable.keys.gpio_rw_value, value);
            cmd->regTable.tables.add(GRegTable.keys.gpio_rw_value, value);
        }

        //Resp
        GSendCmd.head->cmd_id = cmd->head->cmd_id;
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

        //Penet
        if (cmd->regTable.tables.get(GRegTable.keys.penet_data))
            GDevice->onCmd_penet(cmd);
            
        //rfir_send
        if (cmd->regTable.tables.get(GRegTable.keys.rfir_send_data))
            GDevice->onCmd_rfir_send(cmd);

        //set_gpio
        if (cmd->regTable.tables.exist(GRegTable.keys.gpio_rw_pin) &&
            cmd->regTable.tables.exist(GRegTable.keys.gpio_rw_value)) {
            int pin = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_pin);
            int value = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_value);
            rfir::util::Util::SetGpio(pin, value);
        }


        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::set;
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

void service::Cmds::onCmd_get_gpio(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    if (GDevice->onCmd_get_gpio(cmd) != -1){
        uint8_t pin = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_pin);
        int value = rfir::util::Util::GetGpio(pin);   

        //Resp
        GSendCmd.regTable.tables.add(GRegTable.keys.gpio_rw_pin, pin);
        GSendCmd.regTable.tables.add(GRegTable.keys.gpio_rw_value, value);
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::get_gpio;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        std::list<int> ids;
        ids.push_back(GRegTable.keys.gpio_rw_pin);
        ids.push_back(GRegTable.keys.gpio_rw_value);
        GSendCmd.send(ids);
    };
};

void service::Cmds::onCmd_set_gpio(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    if (GDevice->onCmd_set_gpio(cmd) != -1){
        uint8_t pin = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_pin);
        int value = cmd->regTable.tables.get(GRegTable.keys.gpio_rw_value);
        rfir::util::Util::SetGpio(pin, value);

        //Resp
        GSendCmd.regTable.tables.add(GRegTable.keys.gpio_rw_pin, pin);
        GSendCmd.regTable.tables.add(GRegTable.keys.gpio_rw_value, value);
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::set_gpio;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        std::list<int> ids;
        ids.push_back(GRegTable.keys.gpio_rw_pin);
        ids.push_back(GRegTable.keys.gpio_rw_value);
        GSendCmd.send(ids);
    };
};

void service::Cmds::onCmd_rfir_sniff(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    auto err_no = GDevice->onCmd_rfir_sniff(cmd);
    if ( err_no != -1){
        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::rfir_sniff;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        GSendCmd.head->err_no = err_no;
        GSendCmd.send();
    };
};

void service::Cmds::onCmd_rfir_send(cmds::cmd::RecvCmd* cmd){
    GSendCmd.reset();
    auto err_no = GDevice->onCmd_rfir_send(cmd);
    if ( err_no != -1){
        //Resp
        GSendCmd.head->cmd_id = cmds::cmd::CmdId::rfir_send;
        GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
        GSendCmd.head->cmd_stp = 1;
        GSendCmd.head->err_no = err_no;
        GSendCmd.send();
    };
};

void service::Cmds::onCmd_intranet(cmds::cmd::RecvCmd* cmd){    
    if (GDevice->onCmd_intranet(cmd) != -1){
        if (cmd->regTable.tables.getSize() > 0) {
            std::list<int> ids;
            GRegTable.merge(&cmd->regTable, ids);
            LMqttClient.params.enable = GRegTable.tables.get(GRegTable.keys.intranet_mqtt_enable);
            LMqttClient.params.ip = GRegTable.values.intranet_mqtt_ip;
            LMqttClient.params.port = GRegTable.tables.get(GRegTable.keys.intranet_mqtt_port);
            LMqttClient.params.user = strcmp(GRegTable.values.intranet_mqtt_user, "") == 0 ? GRegTable.values.mqtt_user : GRegTable.values.intranet_mqtt_user; 
            LMqttClient.params.pass = strcmp(GRegTable.values.intranet_mqtt_user, "") == 0 ? GRegTable.values.mqtt_pass : GRegTable.values.intranet_mqtt_pass; 
            
            for (auto it = ids.begin(); it != ids.end(); it++)
            {
               GConfig.saved.add(*it, 0);
            }
 
            //
            GConfig.save();  

            //Resp
            if (cmd->head->cmd_stp == 0) {
                GSendCmd.reset();
                GSendCmd.head->cmd_id = cmds::cmd::CmdId::intranet;
                GSendCmd.head->cmd_sid = cmd->head->cmd_sid;
                GSendCmd.head->cmd_stp = 1;
                GSendCmd.send();       
            } 


            //启动内网
            LMqttClient.delayConnectToMqtt();
        }

    };    
};

service::Cmds GCmds;     

               

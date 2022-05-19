#include "cmd.h"
#include "rfir/module/device/device.h"
#include "cmds/cmd/cmd-dispatcher.h"


void service::cmds::Cmd::Start() {
    GCmdDispatcher.events.onCommand.add((void*)&OnCommand, OnCommand);
    // GDevice->events.onChange.add((void*)OnDeviceChange, OnDeviceChange );
    GDevice->events.onEvtPropsChange.add((void*)OnEvt_props_change, OnEvt_props_change);
    GDevice->events.onEvtPenet.add((void*)OnEvt_penet, OnEvt_penet);
}

// void service::cmds::Cmd::Loop() {
//     //Device循环
//     // rfir::service::device::Device::Loop();
// }

// bool service::cmds::Cmd::PublishMsg(const char* msg) {
//     // return network::service::mqtt::Client::Publish(msg);
//     return 0;
// }

// void service::cmds::Cmd::DoTimerReport(bool reset) {
//     GDevice->doTimerReport(reset);
// }


// bool service::cmds::Cmd::OnCmd(const char* cmd) {
//     DEBUGER.print("OnCmd: ");
//     DEBUGER.println(cmd);
//     neb::CJsonObject jcmd;
//     if ((int)cmd != 0 &&  jcmd.Parse(cmd)) {
//         return OnCmd(&jcmd);
//     }
//     return false;     
// }

// bool service::cmds::Cmd::OnCmd(neb::CJsonObject* jcmd) {
//     neb::CJsonObject hd;
//     int cmd = 0;                
//     if (jcmd->Get("hd", hd) && hd.Get("cmd", cmd)) {
//         switch (cmd)
//         {
//             case ECmdId::HeartBeet: //心跳        
//                 return OnCmd_heartbeat(jcmd);
//                 break;
//             case ECmdId::GetSet: //空调控制   
//                 return OnCmd_set(jcmd);
//                 break;            
//             case ECmdId::GetVersion: //获取版本号    
//                 return OnCmd_getversion(jcmd);
//                 break;   
//             case ECmdId::Update: //更新/升级             
//             case ECmdId::Reboot: //重启
//                 return OnCmd_reboot(jcmd, "reboot by user cmd");                
//                 break;
//             case ECmdId::IRSend: //射频发送指令
//             case ECmdId::Send:
//                 return OnCmd_send(jcmd);
//                 break;  
//             case ECmdId::Sniff:
//                 return OnCmd_sniff(jcmd);
//                 break;                  
//             case ECmdId::Codec:
//                 return OnCmd_codec(jcmd);
//                 break;   
//             case ECmdId::Config:
//                 return OnCmd_config(jcmd);
//                 break;                                          
//             default:
//                 break;
//         }        
//     }
    
//     return false;
// }

// bool service::cmds::Cmd::OnCmd_heartbeat(neb::CJsonObject* _cmd, uint8_t st) {
//     neb::CJsonObject cmd, hd, pld;
//     hd.Add("did", rfir::util::Util::GetChipId());
//     hd.Add("cmd", ECmdId::HeartBeet);
//     hd.Add("stp", 1);
//     pld.Add("st", st);
//     pld.Add("ver", OTA_VERSION_NUMBER);
//     pld.Add("rssi", WiFi.RSSI());
//     pld.Add("mac", rfir::util::Util::GetMacAddress());
// #ifdef DEV_FACTURER
//     pld.Add("facturer", DEV_FACTURER);
//     pld.Add("model", DEV_MODEL);
// #endif  
//     cmd.Add("hd", hd);
//     cmd.Add("pld", pld);
    
//     return PublishMsg(cmd.ToString().c_str());
// }

// bool service::cmds::Cmd::OnCmd_reboot(neb::CJsonObject* _doc, std::string reason) {
//     neb::CJsonObject cmd, hd, pld;
//     hd.Add("did", rfir::util::Util::GetChipId());
//     hd.Add("cmd", ECmdId::Reboot);
//     hd.Add("stp", 1);

//     pld.Add("extra", reason);

//     cmd.Add("hd", hd);
//     cmd.Add("pld", pld);
    
//     PublishMsg(cmd.ToString().c_str());

//     delay(5000);
// #ifdef ESP8266    
//     ESP.reset();
// #else 
//     ESP.restart();
// #endif
//     delay(1000);
//     return true;
// }


// bool service::cmds::Cmd::OnCmd_getversion(neb::CJsonObject* _doc) {
//     neb::CJsonObject cmd, hd, pld;
//     hd.Add("did", rfir::util::Util::GetChipId());
//     hd.Add("cmd", ECmdId::GetVersion);
//     hd.Add("stp", 1);

//     pld.Add("version", OTA_VERSION_NUMBER);
//     pld.Add("rssi", WiFi.RSSI());
//     pld.Add("mac", rfir::util::Util::GetMacAddress());

//     cmd.Add("hd", hd);
//     cmd.Add("pld", pld);    
//     return PublishMsg(cmd.ToString().c_str());
// }

// bool service::cmds::Cmd::OnCmd_set(neb::CJsonObject* cmd) {
//     neb::CJsonObject hd, pld;
//     std::string key;
//     int stp;

//     if (!cmd->Get("pld", pld) || !pld.GetKey(key) || (cmd->Get("hd", hd) && hd.Get("stp", stp) && stp == 2))
//         return OnCmd_get(&pld);
//     else {
//         auto result = GDevice->onCmd_set(&pld);
//         DoTimerReport(true);
//         return result;
//     }
// }

// bool service::cmds::Cmd::OnCmd_get(neb::CJsonObject* _pld, std::string reason) {
//     neb::CJsonObject cmd, hd, pld;
//     if (_pld)  pld = *_pld;
//     hd.Add("did", rfir::util::Util::GetChipId());
//     hd.Add("cmd", ECmdId::GetSet);
//     hd.Add("stp", 1);
//     rfir::service::device::Device::OnCmd_get(&pld);
//     pld.Add("reason", reason);

//     cmd.Add("hd", hd);
//     cmd.Add("pld", pld);    
//     return PublishMsg(cmd.ToString().c_str());
// }

// bool service::cmds::Cmd::OnCmd_send(neb::CJsonObject* cmd) {
//     auto rfir = rfir::GetRfir();
//     rfir->sniffer->stop();
//     rfir::service::cmds::Cmd::onCmd(cmd); 
//     rfir->sniffer->start();
//     return true;
// }

// bool service::cmds::Cmd::OnCmd_sniff(neb::CJsonObject* cmd) {
//     auto rfir = rfir::GetRfir();
//     rfir->sniffer->stop();
//     rfir::service::cmds::Cmd::onCmd(cmd); 
//     rfir->sniffer->start();
//     return true;
// }


// bool service::cmds::Cmd::OnCmd_codec(neb::CJsonObject* cmd) {
//     return rfir::service::cmds::Cmd::onCmd(cmd); 
// }

// bool service::cmds::Cmd::OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
//     if (GDevice->onCmd_decoded(data)) {
//         OnCmd_get((neb::CJsonObject*)0, "IR Change");
        
//         //Report
//         DoTimerReport(true);
//         return true;
//     }
//     return false;
// }


// bool service::cmds::Cmd::OnCmd_config(neb::CJsonObject* cmd) {
//     neb::CJsonObject pld, content;
//     std::string fn;
//     if (cmd->Get("pld", pld)) {
//         if (pld.Get("filename", fn)) {

//         }
//     }
//         return OnCmd_getconfig(&pld);
//     return false;
// }

// bool service::cmds::Cmd::OnCmd_getconfig(neb::CJsonObject* cmd) {
//     neb::CJsonObject pld;
//     if (cmd->Get("pld", pld))
//         return rfir::service::device::Device::OnCmd_config(&pld);
//     return false;
// }


// void* service::cmds::Cmd::OnDeviceChange(void* arg, void * p){
//     if (p) 
//         OnCmd_get(0, (const char*)p);
//     else 
//         OnCmd_get(0);
    
//     return 0;
// };

// bool  service::cmds::Cmd::OnCmd_get(::cmds::cmd::CmdMqtt* reqCmd, std::string reason){
//     ::cmds::cmd::CmdMqtt cmd;
//     neb::CJsonObject& hd = cmd.command.hd;
//     neb::CJsonObject& pld = cmd.command.pld;

//     GDevice->onCmd_get(&pld);
//     hd.ReplaceAdd("reason", reason);

//     if (reqCmd){
//         cmd.command.head = reqCmd->command.head;
//         cmd.command.head.from = reqCmd->command.head.to;
//         cmd.command.head.to = reqCmd->command.head.from;
//         cmd.command.head.stp = 1;
//     }

//     cmd.command.head.entry.type = "evt";
//     cmd.command.head.entry.id = ::Config.mqtt_dev_evt_report;
//     return cmd.send();
// };

// bool  service::cmds::Cmd::OnCmd_set(::cmds::cmd::CmdMqtt* cmd){
//     auto result = GDevice->onCmd_set(&cmd->command.pld);
//     GDevice->doTimerReport(true);
//     return result;    
// };


void* service::cmds::Cmd::OnCommand(void* arg, void * p){    
    auto cmd = (::cmds::cmd::CmdMqtt*)p;
    DEBUGER.printf("service::cmds::Cmd::OnCommand: %s %s %s \r\n", cmd->topic.c_str(), cmd->command.head.entry.type.c_str(), cmd->command.head.entry.id.c_str() );


    //服务请求
    if (cmd->command.head.stp == 0) {
        if (cmd->command.head.entry.type == "svc") {
            if (cmd->command.head.entry.id == "get") {
                return (void*)OnSvc_get(cmd);
            }

            if (cmd->command.head.entry.id == "set") {
                return (void*)OnSvc_set(cmd);
            }
        }

        if (cmd->topic == ::Config.mqtt_dev_svc_penet) {
            return (void*)OnSvc_penet(cmd);
        }
    }



    //服务响应
    if (cmd->command.head.stp == 1) {
        if (cmd->topic == ::Config.mqtt_edg_svc_handshake) {
            return (void*)OnSvc_handshake_resp(cmd);
        }
    }


    return 0;
};


bool  service::cmds::Cmd::OnSvc_get(::cmds::cmd::CmdMqtt* reqCmd, std::string reason){
    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    auto result = GDevice->getProps(&pld);
    pld.ReplaceAdd("_extra", reason);
    pld.ReplaceAdd("_success", result);

    if (reqCmd){
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    }

    result = cmd.send();
    GDevice->doEvtTimerReport(1000);
    return result;
};

bool  service::cmds::Cmd::OnSvc_set(::cmds::cmd::CmdMqtt* reqCmd, std::string reason){
    auto result = GDevice->onSvc_set(&reqCmd->command.pld);

    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    GDevice->getProps(&pld);
    pld.ReplaceAdd("_extra", reason);
    pld.ReplaceAdd("_success", result);

    if (reqCmd){
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    }


    result = cmd.send();
    GDevice->doEvtTimerReport(1000);
    return result;
};

bool  service::cmds::Cmd::OnSvc_penet(::cmds::cmd::CmdMqtt* cmd){
    return  GDevice->onSvc_penet(&cmd->command.pld);
};

bool  service::cmds::Cmd::OnSvc_handshake_resp(::cmds::cmd::CmdMqtt* cmd){
    return  GDevice->doEvtTimerReport(1000);
};

void*  service::cmds::Cmd::OnEvt_props_change(void* arg, void* p){
    if (p)
        OnSvc_get(0, (char*)p);
    else
        OnSvc_get(0);
    return 0;
};

void*  service::cmds::Cmd::OnEvt_penet(void* arg, void* p){
    if (p) {
        ::cmds::cmd::CmdMqtt cmd;
        neb::CJsonObject& hd = cmd.command.hd;
        neb::CJsonObject& pld = cmd.command.pld;
        pld = *((neb::CJsonObject*)p);

        cmd.command.head.entry.type = "evt";
        cmd.command.head.entry.id = ::Config.mqtt_dev_evt_penet;
        cmd.send();
    }
    return 0;
};
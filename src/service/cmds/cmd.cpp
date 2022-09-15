#include "cmd.h"
#include "rfir/module/device/device.h"
#include "rfir/module/device/networking.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"


void service::cmds::Cmd::Start() {
    GCmdDispatcher.events.onConnect.add((void*)&OnConnect, OnConnect);
    GCmdDispatcher.events.onCommand.add((void*)&OnCommand, OnCommand);
    GDevice->events.onEvtPropsChange.add((void*)OnEvt_props_change, OnEvt_props_change);
    GDevice->events.onEvtPenet.add((void*)OnEvt_penet, OnEvt_penet);
}

void* service::cmds::Cmd::OnConnect(void* arg, void * p){
    GNetworking.delayHandshake(500);
    GDevice->doEvtTimerReport(1000);
    return 0;
};

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

            if (cmd->command.head.entry.id == "reboot") {
                return (void*)OnSvc_reboot(cmd);
            }

            if (cmd->command.head.entry.id == "penet") {
                return (void*)OnSvc_penet(cmd);
            }

            if (cmd->command.head.entry.id == "config") {
                return (void*)OnSvc_config(cmd);
            }
        }

    }



    //服务响应
    if (cmd->command.head.stp == 1) {
        if (cmd->command.head.entry.type == "svc") {
            if (cmd->command.head.entry.id == "handshake") {
                return (void*)OnSvc_handshake_resp(cmd);
            }
        }
    }


    return 0;
};


int  service::cmds::Cmd::OnSvc_get(::cmds::cmd::CmdMqtt* reqCmd, std::string reason){
    ESP.resetFreeContStack();
    ESP.resetHeap();    
    
    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    auto result = GDevice->getProps(&pld, reqCmd);
    pld.ReplaceAdd("_extra", reason);
    pld.ReplaceAdd("_success", result);

    if (reqCmd){
        hd = reqCmd->command.hd;
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    } else {
        cmd.command.head.entry.type = "evt";
        cmd.command.head.entry.id = "report";
    }
    Config.getIds(&hd);

    result = cmd.send();
    GDevice->doEvtTimerReport(1000);
    return result;
};

int  service::cmds::Cmd::OnSvc_set(::cmds::cmd::CmdMqtt* reqCmd, std::string reason){
    auto result = GDevice->onSvc_set(&reqCmd->command.pld, reqCmd);
    if (result != -1) {
        ::cmds::cmd::CmdMqtt cmd;
        neb::CJsonObject& hd = cmd.command.hd;
        neb::CJsonObject& pld = cmd.command.pld;

        GDevice->getProps(&pld, reqCmd);
        pld.ReplaceAdd("_extra", reason);
        pld.ReplaceAdd("_success", result);

        if (reqCmd){
            hd = reqCmd->command.hd;
            cmd.command.head = reqCmd->command.head;
            cmd.command.head.from = reqCmd->command.head.to;
            cmd.command.head.to = reqCmd->command.head.from;
            cmd.command.head.stp = 1;
        }
        Config.getIds(&hd);

        result = cmd.send();
        GDevice->doEvtTimerReport(1000);
    }
    return result;
};

int  service::cmds::Cmd::OnSvc_reboot(::cmds::cmd::CmdMqtt* reqCmd){
    auto result = GDevice->onSvc_reboot(&reqCmd->command.pld, reqCmd);

    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;

    GDevice->getProps(&pld, reqCmd);
    pld.ReplaceAdd("_success", result);
    if (reqCmd){
        hd = reqCmd->command.hd;
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    }
    Config.getIds(&hd);

    result = cmd.send();
    return result;
};

int  service::cmds::Cmd::OnSvc_penet(::cmds::cmd::CmdMqtt* cmd){
    return  GDevice->onSvc_penet(&cmd->command.pld, cmd);
};

int  service::cmds::Cmd::OnSvc_handshake_resp(::cmds::cmd::CmdMqtt* cmd){
    return  GDevice->doEvtTimerReport(1000);
};

int  service::cmds::Cmd::OnSvc_config(::cmds::cmd::CmdMqtt* reqCmd){
    neb::CJsonObject* reqPld = &reqCmd->command.pld;
    neb::CJsonObject config;
    ::cmds::cmd::CmdMqtt cmd;
    neb::CJsonObject& hd = cmd.command.hd;
    neb::CJsonObject& pld = cmd.command.pld;
    int result = 0;


    if (reqPld->Get("config", config)) {
        result = Config.saveToFile(config);
        GDevice->onSvc_config(&reqCmd->command.pld, reqCmd);
        pld.ReplaceAdd("_extra", "Config Set");
    } else {
        pld.ReplaceAdd("_extra", "Config Get");        
    }

    result = Config.loadFromFile(config);
    pld.ReplaceAdd("config", config);
    pld.ReplaceAdd("_success", result);

    if (reqCmd){
        hd = reqCmd->command.hd;
        cmd.command.head = reqCmd->command.head;
        cmd.command.head.from = reqCmd->command.head.to;
        cmd.command.head.to = reqCmd->command.head.from;
        cmd.command.head.stp = 1;
    }
    
    result = cmd.send();

    int reboot = 0;
    reqPld->Get("reboot", reboot);
    if (reboot)
        rfir::util::Util::DelayReset(3000);

    return result;
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
        cmd.command.head.entry.id = "penet";
        cmd.send();
    }
    return 0;
};
#ifndef __SERVICE_CMDS_CMD_H__
#define __SERVICE_CMDS_CMD_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/util/util.h"
#include "cmds/cmd/cmd-mqtt.h"

namespace service {
    namespace cmds {
        class Cmd {    
        public:
            static void Start();
            static void Loop();
        public:
            static void* OnConnect(void* arg, void * p);
            static void* OnCommand(void* arg, void * p);
        public:
            static int  OnSvc_get(::cmds::cmd::CmdMqtt* cmd, std::string reason = "User Get");
            static int  OnSvc_set(::cmds::cmd::CmdMqtt* cmd, std::string reason = "User Set");
            static int  OnSvc_reboot(::cmds::cmd::CmdMqtt* cmd);
            static int  OnSvc_penet(::cmds::cmd::CmdMqtt* cmd);
            static int  OnSvc_handshake_resp(::cmds::cmd::CmdMqtt* cmd);
            static int  OnSvc_config(::cmds::cmd::CmdMqtt* cmd);
            
        public:
            static void*  OnEvt_props_change(void* arg, void* p);
            static void*  OnEvt_penet(void* arg, void* p);
            
        };
    }
}


#endif //__SERVICE_CMDS_CMD_H__


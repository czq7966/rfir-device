#ifndef __SERVICE_CMDS_CMD_H__
#define __SERVICE_CMDS_CMD_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/util/util.h"
#include "rfir/service/cmds/cmd.h"
#include "cmds/cmd/cmd-mqtt.h"
// #include "network/service/mqtt/client.h"

namespace service {
    namespace cmds {
        class Cmd {    
        public:      
            // enum ECmdId {
            //     HeartBeet = 1,  //心跳指令
            //     IRSend = 2, //红外发送指令
            //     GetSet = 3,  //状态查询/设置指令
            //     Curtain = 4, //窗帘指令
            //     GetVersion = 5, //获取版本指令
            //     Update = 6, //升级指令
            //     Reboot = 7, //重启指令
            //     Send = 8, //射频发送指令
            //     Sniff = 9, //射频采集指令
            //     Codec = 10, //射频编解码指令
            //     Config = 11 //修改配置指令
            // };
        public:
            static void Start();
            static void Loop();
        // public:       
        //     static bool PublishMsg(const char* msg);
        //     static void DoTimerReport(bool reset = false);
        // public:
        //     static bool OnCmd(const char* cmd);
        //     static bool OnCmd(neb::CJsonObject* cmd);
        //     static bool OnCmd_heartbeat(neb::CJsonObject* cmd, uint8_t st = 0);
        //     static bool OnCmd_reboot(neb::CJsonObject* cmd, std::string reason);
        //     static bool OnCmd_getversion(neb::CJsonObject* cmd);
        //     static bool OnCmd_set(neb::CJsonObject* cmd);
        //     static bool OnCmd_get(neb::CJsonObject* cmd, std::string reason = "User Get");
        //     static bool OnCmd_send(neb::CJsonObject* cmd);
        //     static bool OnCmd_sniff(neb::CJsonObject* cmd);
        //     static bool OnCmd_codec(neb::CJsonObject* cmd);
        //     static bool OnCmd_config(neb::CJsonObject* cmd);
        //     static bool OnCmd_getconfig(neb::CJsonObject* cmd);
        // public:
        //     static bool OnCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data);
        public:
            static void* OnCommand(void* arg, void * p);
            // static void* OnDeviceChange(void* arg, void * p);

            static bool  OnCmd_get(::cmds::cmd::CmdMqtt* cmd, std::string reason = "User Get");
            static bool  OnCmd_set(::cmds::cmd::CmdMqtt* cmd);

        public:
            static bool  OnSvc_get(::cmds::cmd::CmdMqtt* cmd, std::string reason = "User Get");
            static bool  OnSvc_set(::cmds::cmd::CmdMqtt* cmd);
            static bool  OnSvc_penet(::cmds::cmd::CmdMqtt* cmd);
        public:
            static void*  OnEvt_props_change(void* arg, void* p);
            static void*  OnEvt_penet(void* arg, void* p);
            
        };
    }
}


#endif //__SERVICE_CMDS_CMD_H__


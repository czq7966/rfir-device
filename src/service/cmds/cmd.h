#ifndef __SERVICE_CMDS_CMD_H__
#define __SERVICE_CMDS_CMD_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/util/util.h"
#include "rfir/service/cmds/cmd.h"
#include "network/service/mqtt/client.h"
#include "module/ac/mcquay.h"

namespace service {
    namespace cmds {
        class Cmd {    
        public:      
            enum ECmdId {
                HeartBeet = 1,
                ACSet = 3,
                GetVersion = 5,
                Update = 6,
                Reboot = 7,
                Send = 8, 
                Codec = 10
            };
        public:       
            static bool PublishMsg(const char* msg);
            static bool OnCmd(const char* cmd);
            static bool OnCmd(neb::CJsonObject* cmd);
            static bool OnCmd_heartbeat(neb::CJsonObject* cmd, uint8_t st = 0);
            static bool OnCmd_reboot(neb::CJsonObject* cmd, std::string reason);
            static bool OnCmd_getversion(neb::CJsonObject* cmd);
            static bool OnCmd_ac_set(neb::CJsonObject* cmd);
            static bool OnCmd_ac_get(neb::CJsonObject* cmd, std::string reason = "User Get");
            static bool OnCmd_send(neb::CJsonObject* cmd);
            static bool OnCmd_Codec(neb::CJsonObject* cmd);
            static bool On_Decoded(uint8_t* bytes, uint16_t nbits);

        };
    }
}


#endif //__SERVICE_CMDS_CMD_H__


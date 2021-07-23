//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_CMDS_CMD_H__
#define __RFIR_SERVICE_CMDS_CMD_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/cmds/cmd.h"
#include "rfir/module/ttl/sniffer.h"
#include "rfir/module/ttl/sender.h"

namespace rfir {
    namespace service {
        namespace cmds {
            class Cmd {          
            public:      
                static rfir::module::cmds::Cmd* cmd;
                static bool onCmd(const char * cmd);
                static void onCmd(neb::CJsonObject* cmd);
                static void onCmd_send(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static void onCmd_send(rfir::module::ttl::Sender::Params p, const char* data, int size);
                static void onCmd_sniff(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static void onCmd_sniff(rfir::module::ttl::Sniffer::Params p);
                
                static void onCmd_decode(neb::CJsonObject* hd, neb::CJsonObject* pld);
            };
        }
    }
}


#endif //__RFIR_SERVICE_CMDS_CMD_H__


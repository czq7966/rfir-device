//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_CMDS_CMD_H__
#define __RFIR_SERVICE_CMDS_CMD_H__

#include "rfir/rfir.h"
#include "rfir/module/cmds/cmd.h"

namespace rfir {
    namespace service {
        namespace cmds {
            class Cmd {          
            public:      
                static rfir::module::cmds::Cmd* cmd;
                static bool onCmd(const char * cmd);
                static bool onCmd(neb::CJsonObject* cmd);
                static bool onCmd_restart(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static bool onCmd_send(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static bool onCmd_send(rfir::module::ttl::Config::Device* d);
                static bool onCmd_sendRaw(rfir::module::ttl::Config::Device* d, const char* data, int size);
                static bool onCmd_sendEncode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* encode);
                static bool onCmd_sniff(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static bool onCmd_sniff(rfir::module::ttl::Sniffer::Params p, const char* name);
                static bool onCmd_sniff(rfir::module::ttl::Config::Device* device, bool start = false);
                
                static bool onCmd_decode_encode(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static bool onCmd_decode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* decode);
                static bool onCmd_encode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* encode);

                static bool onCmd_config(neb::CJsonObject* hd, neb::CJsonObject* pld);
                static bool onCmd_config_change(neb::CJsonObject* conent);
                static bool onCmd_config_save(neb::CJsonObject* content);
                static bool onCmd_config_delete();

            };
        }
    }
}


#endif //__RFIR_SERVICE_CMDS_CMD_H__


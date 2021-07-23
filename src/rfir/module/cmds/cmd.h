//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_CMDS_CMD_H__
#define __RFIR_MODULE_CMDS_CMD_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include <list>
#include <mutex>


namespace rfir {
    namespace module {
        namespace cmds {
            class Cmd {
                //std::mutex m_mutex;                
                std::list<neb::CJsonObject*> cmds;                
            public:
                bool pushCmd(neb::CJsonObject* cmd);
                bool pushCmd(const char* cmdStr);
                bool popCmd(neb::CJsonObject*& cmd);
            };
        }
    }
}


#endif //__RFIR_MODULE_CMDS_CMD_H__

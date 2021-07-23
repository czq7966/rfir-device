//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_SERIAL_SENDER_H__
#define __RFIR_MODULE_SERIAL_SENDER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include <list>
#include <mutex>

namespace rfir {
    namespace module {
        namespace serial {
            class Sender {
                //std::mutex m_mutex;
            public:
                std::list<neb::CJsonObject*> cmds;
                bool pushCmd(neb::CJsonObject* cmd) {
                    //std::lock_guard<std::mutex> lock(m_mutex);
                    cmds.push_back(cmd);
                    return true;
                }
                 
                bool popCmd(neb::CJsonObject*& cmd) {
                    //std::lock_guard<std::mutex> lock(m_mutex);
                    if (cmds.size() >0) {
                        cmd = cmds.front();
                        cmds.pop_front();
                        return true;
                    }
                    return false;
                }

                bool dealCmd() {
                    neb::CJsonObject* cmd;
                    while (popCmd(cmd)) {
                        Serial.print(cmd->ToString().c_str());
                        Serial.print("\n");
                        delete cmd;
                        cmd = 0;
                    }
                    return 1;
                }
            };
        }
    }
}


#endif //__RFIR_MODULE_SERIAL_RECEIVER_H__

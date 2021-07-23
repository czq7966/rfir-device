//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_SERIAL_RECEIVER_H__
#define __RFIR_MODULE_SERIAL_RECEIVER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include <list>
#include <mutex>


namespace rfir {
    namespace module {
        namespace serial {
            class Receiver {
                //std::mutex m_mutex;                
            public:
                std::list<neb::CJsonObject*> cmds;
                void dealCmd() {
                    int c = Serial.read();
                    while (c > 0) {
                        if (c > 0 && (char)c != '\n') {
                            String str = (char)c + Serial.readStringUntil('\n');
                            pushCmd(str.c_str());
                        }
                        c = Serial.read();
                    }
                }

                bool pushCmd(neb::CJsonObject* cmd) {
                    //std::lock_guard<std::mutex> lock(m_mutex);
                    cmds.push_back(cmd);
                    return true;
                }

                bool pushCmd(const char* cmdStr) {
                            //std::string jsonStr(cmdStr.c_str());
                    neb::CJsonObject* cmd = new neb::CJsonObject();
                    if (cmd->Parse(cmdStr)) {
                        return pushCmd(cmd);
                    } else {
                        delete cmd;
                    }

                    return false;
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
            };
        }
    }
}


#endif //__RFIR_MODULE_SERIAL_RECEIVER_H__

#include "cmd.h"

bool rfir::module::cmds::Cmd::pushCmd(neb::CJsonObject* cmd) {
    //std::lock_guard<std::mutex> lock(m_mutex);
    cmds.push_back(cmd);
    return true;
}

bool rfir::module::cmds::Cmd::pushCmd(const char* cmdStr) {
            //std::string jsonStr(cmdStr.c_str());
    neb::CJsonObject* cmd = new neb::CJsonObject();
    if (cmd->Parse(cmdStr)) {
        return pushCmd(cmd);
    } else {
        delete cmd;
    }

    return false;
}                

bool rfir::module::cmds::Cmd::popCmd(neb::CJsonObject*& cmd) {
    //std::lock_guard<std::mutex> lock(m_mutex);
    if (cmds.size() >0) {
        cmd = cmds.front();
        cmds.pop_front();
        return true;
    }
    return false;
}
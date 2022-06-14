#include "cl-xz-msg-gree-485.h"


void rfir::module::device::ac::CL_XZ_MSG_GREE_485::init() {
    this->name = "CL_XZ_MSG_GREE_485";
}

bool rfir::module::device::ac::CL_XZ_MSG_GREE_485::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string innerCode = "0xA0 01 00 00 00 00 00 04";
    std::string code;
    if (!pld->Get("code", code) || code.length() == 0) {
        pld->ReplaceAdd("code", innerCode);
    }

    return RS485::onSvc_get(pld, cmd);    
}
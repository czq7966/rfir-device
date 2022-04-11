#include "cl-vrhall-f1-ac-mcquay-485.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485::init() {
    auto d = RS485::init();
    if (!d) return 0;

   this->name = "mcquay_485";

    return d;
}

bool rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485::onCmd_get(neb::CJsonObject* pld) {
    std::string innerCode = "0x03 04 13 87 00 19 84 8F";
    std::string code;
    if (!pld->Get("code", code) || code.length() == 0) {
        pld->ReplaceAdd("code", innerCode);
    }

    return RS485::onCmd_get(pld);    
}
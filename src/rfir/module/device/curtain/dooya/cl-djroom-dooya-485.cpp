#include "cl-djroom-dooya-485.h"


rfir::module::ttl::Config::Device* rfir::module::device::curtain::CL_DJROOM_Dooya_485::init() {
    auto d = RS485::init();
    if (!d) return 0;

   this->name = "dooya";

    return d;
}


bool rfir::module::device::curtain::CL_DJROOM_Dooya_485::onCmd_get(neb::CJsonObject* pld) {
    std::string innerCode = "0x55 FE FE 01 00 10 44 2E";
    std::string code;
    if (!pld->Get("code", code) || code.length() == 0) {
        pld->ReplaceAdd("code", innerCode);
    }

    return RS485::onCmd_get(pld);
}
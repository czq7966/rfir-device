#include "cl-vrhall-f1-ac-mcquay-485.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485::init() {
    auto d = RS485::init();
    if (!d) return 0;

   this->name = "mcquay_485";

    return d;
}


#ifndef __RFIR_MODULE_TTL_DEVICE_GREE_H__
#define __RFIR_MODULE_TTL_DEVICE_GREE_H__

#include "../config.h"
#include "../rfir.h"

namespace rfir {
    namespace module {
        namespace ttl {
            namespace device {
                class Mcquay {
                public:
                    static rfir::module::ttl::Config::Device* Init(rfir::module::ttl::Config::Devices* ds, std::string name = "mcquay");
                };
            }
       
       }
    }
}


#endif //__RFIR_MODULE_TTL_DEVICE_GREE_H__
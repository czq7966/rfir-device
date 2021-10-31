#ifndef __RFIR_SERVICE_DEVICE_AC_GREE_H__
#define __RFIR_SERVICE_DEVICE_AC_GREE_H__


#include <Arduino.h>
#include "config-rfir.h"
#include "rfir/rfir.h"
#include "ir_Gree.h"

namespace rfir {
    namespace service {
        namespace device {
            namespace ac {
                class Gree {
                public:
                    static IRGreeAC* Device;
                    static rfir::module::ttl::Config::Device* Init(std::string name = "gree");
                    static bool OnCmd(const char* cmd);
                    static bool OnCmd(neb::CJsonObject* cmd);
                    // static rfir::module::ttl::Encoder::EncodeResult* getRaw();
                    static uint16_t* getEncodeRaw(uint16_t& count);
                };
            }
        }
    }
}



#endif //

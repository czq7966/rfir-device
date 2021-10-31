#ifndef __RFIR_SERVICE_DEVICE_AC_MCQUAY_H__
#define __RFIR_SERVICE_DEVICE_AC_MCQUAY_H__


#include <Arduino.h>
#include "config-rfir.h"
#include "rfir/rfir.h"


namespace rfir {
    namespace service {
        namespace device {
            namespace ac {
                class Mcquay {
                public:
                    static rfir::module::ttl::Config::Device* Init(std::string name = "mcquay");
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

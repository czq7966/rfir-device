#ifndef __RFIR_MODULE_DEVICE_TEST_SNIFFER_ESP32_WROOM__
#define __RFIR_MODULE_DEVICE_TEST_SNIFFER_ESP32_WROOM__


#include "sniffer.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace test {
                class Sniffer_esp32_wroom: public Sniffer {
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                };
            }
        }
    }
}



#endif //

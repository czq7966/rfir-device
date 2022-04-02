#ifndef __NETWORK_SERVICE_WIFI_SMC_H__
#define __NETWORK_SERVICE_WIFI_SMC_H__


#include "network/module/wifi/smc.h"

namespace network {
    namespace service {
        namespace wifi {
            class SMC {
            private:
                static network::module::wifi::SMC* smc;

            public:
                static void Start(module::wifi::SMC::Params* p);
                static void Loop();

            };
        }
    }
}

#endif
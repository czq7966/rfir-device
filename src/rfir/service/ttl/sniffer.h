//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_TTL_SNIFFER_H__
#define __RFIR_SERVICE_TTL_SNIFFER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/sniffer.h"
#include <list>
#include <mutex>



namespace rfir {
    namespace service {
        namespace ttl {
            class Sniffer {
            public:
                static rfir::module::ttl::Sniffer* ttlsniffer;
                static void init(rfir::module::ttl::Sniffer::Params Params);
                static int  sniff();

                static void start();
                static void stop();
                static void gpio_interrupt();   

            };
        }
    }
}


#endif //__RFIR_SERVICE_TTL_SNIFFER_H__

//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_TTL_SENDER_H__
#define __RFIR_SERVICE_TTL_SENDER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/sender.h"
#include <list>
#include <mutex>



namespace rfir {
    namespace service {
        namespace ttl {
            class Sender {
            public:
                static rfir::module::ttl::Sender* ttlsender;
                static void init(rfir::module::ttl::Sender::Params Params);
                static int  send();

            };
        }
    }
}


#endif //__RFIR_SERVICE_TTL_SENDER_H__

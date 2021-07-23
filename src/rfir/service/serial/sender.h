//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_SERIAL_SENDER_H__
#define __RFIR_SERVICE_SERIAL_SENDER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/serial/sender.hpp"

namespace rfir {
    namespace service {
        namespace serial {
            class Sender {
            public:
                static rfir::module::serial::Sender* sender;
                static void dealCmd();
                static bool pushCmd(neb::CJsonObject* cmd);
                static bool pushCmd(const char* topic, const char* payload);
                static bool popCmd(neb::CJsonObject* cmd);
                static bool error(const char* msg);
            };
        }
    }
}


#endif //__RFIR_MODULE_SERIAL_RECEIVER_H__

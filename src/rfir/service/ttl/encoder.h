//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_TTL_ENCODER_H__
#define __RFIR_SERVICE_TTL_ENCODER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/encoder.h"

namespace rfir {
    namespace service {
        namespace ttl {
            class Encoder {
            public:
                static rfir::module::ttl::Encoder* ttlencoder;

            };
        }
    }
}


#endif //__RFIR_SERVICE_TTL_DECODER_H__

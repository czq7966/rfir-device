//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_SERVICE_TTL_DECODER_H__
#define __RFIR_SERVICE_TTL_DECODER_H__

#include "rfir/util/platform.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/decoder.h"
#include <list>
#include <mutex>



namespace rfir {
    namespace service {
        namespace ttl {
            class Decoder {
            public:
                static rfir::module::ttl::Decoder* ttldecoder;
                static void decode(rfir::module::ttl::Decoder* decoder = NULL);

            };
        }
    }
}


#endif //__RFIR_SERVICE_TTL_DECODER_H__

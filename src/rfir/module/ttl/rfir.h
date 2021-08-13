#ifndef __RFIR_MODULE_TTL_RFIR_H__
#define __RFIR_MODULE_TTL_RFIR_H__

#include "../../util/platform.h"
#include "../../util/cjson/CJsonObject.hpp"
#include "../../util/file.h"
#
#include "sniffer.h"
#include "decoder.h"
#include "sender.h"
#include "encoder.h"
#include "config.h"


namespace rfir {
    namespace module {
        namespace ttl {
            class RFIR {    
            public:
                RFIR();
                ~RFIR();
                void init();
                void uninit();
            public:
                Config*  config = 0;
                Sniffer* sniffer = 0;
                Decoder* decoder = 0;
                Sender*  sender = 0;
                Encoder* encoder = 0;                    
            };
        }
    }

}

#endif
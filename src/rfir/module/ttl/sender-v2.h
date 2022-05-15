#ifndef __RFIR_MODULE_TTL_SENDER_V2_H__
#define __RFIR_MODULE_TTL_SENDER_V2_H__

#include "rfir/util/event-emitter.h"


namespace rfir {
    namespace module {
        namespace ttl {
            class SenderV2 {     
            public:
                struct Params {
                    uint8_t  pin = 0;
                    bool     inverted = false;
                    bool     modulation = true;
                    uint16_t repeat = 0;
                    uint16_t frequency = 38;                    
                    uint8_t  dutycycle = 50;

                    std::string   toString();
                };  

                struct Events {
                  rfir::util::Event onSended;
                };

            public:
                std::string name;
                Params params;
                Events events;
            public:
                void sendRaw(const char* data, const int size);
                void sendRaw(std::list<uint16_t>& deltas);
            };
        }
    }
}

extern rfir::module::ttl::SenderV2 GTTLSender;

#endif //__RFIR_MODULE_TTL_SENDER_H__

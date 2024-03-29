#ifndef __RFIR_MODULE_GPIO_TTL_SNIFFER_V2_H__
#define __RFIR_MODULE_GPIO_TTL_SNIFFER_V2_H__

#include "rfir/util/platform.h"

namespace rfir {
    namespace module {
        namespace ttl {
            class SnifferV2 {
            public:
                struct TimeTTL {
                    bool    ttl = 0;
                    timeval time = {};
                };
                struct Data {
                    TimeTTL                 first_ttl;
                    TimeTTL                 last_ttl;
                    std::list<TimeTTL>      ttls;
                    std::list<uint16_t>     deltas;
                    std::string toString();
                    int toBuffer(uint16_t* buffer);
                };

                struct Params {
                    bool        enabled = true;
                    uint8_t     pin = 0;
                    uint8_t     mode = INPUT;
                    bool        inverted = false;
                    uint16_t    minCount = 50;
                    uint16_t    maxCount = 256;
                    uint32_t    minDelta = 150;
                    uint32_t    maxDelta = UINT16_MAX;
                    std::string toString();
                };    

                struct Events {
                    rfir::util::Event onSniffed;
                };    

            public:
                bool m_started = false;
            public:
                void start();
                void loop();

            public:
                std::string name;
                Params  params;
                Events  events;
                Data    data;
               
                void startSniff();
                void stopSniff();
                bool sniffStarted();
            public:
                void pushTTL(bool ttl);
                void sniffDelta();
                void _sniffDelta(TimeTTL ttl);
                std::string  toString();
            public:
                void* onPinInterrupt(void* arg, void* p);
            };
        }
    }
}

extern rfir::module::ttl::SnifferV2 GTTLSniffer;

#endif //__RFIR_MODULE_GPIO_TTL_SNIFFER_H__

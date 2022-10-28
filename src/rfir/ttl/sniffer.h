#ifndef RFIR_TTL_SNIFFER_H
#define RFIR_TTL_SNIFFER_H

#include "Arduino.h"
#include "rfir/util/event-emitter.h"

namespace rfir {
    namespace ttl {
        class Sniffer {
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
            };    

            struct Events {
                rfir::util::Event onSniffed;
            };    

        public:
            bool m_started = false;
        public:
            void start(bool sniff = false);
            void stop();
            void loop();

        public:
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

extern rfir::ttl::Sniffer GTTLSniffer;

#endif 

#ifndef RFIR_TTL_SENDER_H
#define RFIR_TTL_SENDER_H

#include "rfir/util/event-emitter.h"


namespace rfir {
    namespace ttl {
        class Sender {     
        public:
            struct Params {
                bool     enabled = true;
                int      pin = -1;
                bool     inverted = false;
                bool     modulation = true;
                uint16_t repeat = 0;
                uint16_t frequency = 38;                    
                uint8_t  dutycycle = 50;

            };  

            struct Events {
                rfir::util::Event onSended;
            };

        public:
            Params params;
            Events events;
        public:
            void sendRaw(const char* data, const int size);
            void sendRaw(const uint16_t* data, const int size);
            void sendRaw(std::list<uint16_t>& deltas);
        };
    }

}

extern rfir::ttl::Sender GTTLSender;

#endif 

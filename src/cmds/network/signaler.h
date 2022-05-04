#ifndef __CMDS_NETWORK_SIGNALER_H
#define __CMDS_NETWORK_SIGNALER_H

#include "rfir/util/event-emitter.h"
#include "config.h"

namespace cmds {
    namespace network {
        class Signaler {
            struct Events {
                rfir::util::Event onConnect;        
                rfir::util::Event onDisconnect;     
                rfir::util::Event onMessage;        
                rfir::util::Event onDestroy;    
            };

        public:
            Events events;
            ~Signaler();
            virtual int write(void* p);

            void* onConnect(void* arg, void* p);
            void* onDisconnect(void* arg, void* p);
            void* onMessage(void* arg, void* p);

        };
    }
}


#endif 

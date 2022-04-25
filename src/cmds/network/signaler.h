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

            static void* OnConnect(void* arg, void* p);
            static void* OnDisconnect(void* arg, void* p);
            static void* OnMessage(void* arg, void* p);

        };
    }
}


#endif 

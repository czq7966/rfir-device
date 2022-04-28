
#include "signaler.h"

cmds::network::Signaler::~Signaler() {
    events.onDestroy.emit(this);
}


int cmds::network::Signaler::write(void* p) {
    DEBUGER.println("cmds::network::Signaler::write");
    return 0;
};


void* cmds::network::Signaler::OnConnect(void* arg, void* p){  
    DEBUGER.println("cmds::network::Signaler::OnConnect");
    
    auto signaler = (Signaler*)arg;
    return signaler->events.onConnect.emit(p);
    return 0;
};

void* cmds::network::Signaler::OnDisconnect(void* arg, void* p){
    auto signaler = (Signaler*)arg;
    return signaler->events.onDisconnect.emit(p);
    return 0;
    
};
void* cmds::network::Signaler::OnMessage(void* arg, void* p){    
    auto signaler = (Signaler*)arg;
    return signaler->events.onMessage.emit(p);
    return 0;
};
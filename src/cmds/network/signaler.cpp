
#include "signaler.h"

cmds::network::Signaler::~Signaler() {
    events.onDestroy.emit(this);
}


int cmds::network::Signaler::write(void* p) {
    DEBUGER.println("cmds::network::Signaler::write");
    return 0;
};


void* cmds::network::Signaler::onConnect(void* arg, void* p){  
    DEBUGER.println("cmds::network::Signaler::OnConnect");
    return this->events.onConnect.emit(p);

    
    // auto signaler = (Signaler*)arg;
    // return signaler->events.onConnect.emit(p);
    // return 0;
};

void* cmds::network::Signaler::onDisconnect(void* arg, void* p){
    return this->events.onDisconnect.emit(p);
    // auto signaler = (Signaler*)arg;
    // return signaler->events.onDisconnect.emit(p);
    // return 0;
    
};
void* cmds::network::Signaler::onMessage(void* arg, void* p){   
    return this->events.onMessage.emit(p); 
    // auto signaler = (Signaler*)arg;
    // return signaler->events.onMessage.emit(p);
    // return 0;
};
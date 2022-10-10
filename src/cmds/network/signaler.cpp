
#include "signaler.h"

cmds::network::Signaler::~Signaler() {
    events.onDestroy.emit(this);
}


int cmds::network::Signaler::write(void* p) {
    return 0;
};


void* cmds::network::Signaler::onConnect(void* arg, void* p){  
    return this->events.onConnect.emit(p);
};

void* cmds::network::Signaler::onDisconnect(void* arg, void* p){
    return this->events.onDisconnect.emit(p);   
};
void* cmds::network::Signaler::onMessage(void* arg, void* p){   
    return this->events.onMessage.emit(p); 
};
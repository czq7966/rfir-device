#include "mem.h"

void rfir::util::Mem::start(){
};


void rfir::util::Mem::loop(){
    lowCheck();
};

void rfir::util::Mem::lowCheck(){
    auto p = malloc(MQTT_BUFFER_SIZE);
    if (p) 
        free(p);
    else
        this->events.lower.emit(this);
};


rfir::util::Mem GMem;

#include "mem.h"

void rfir::util::Mem::start(){
};


void rfir::util::Mem::loop(){
    lowCheck();
};

void rfir::util::Mem::lowCheck(){
    auto p = malloc(this->lowSize);
    if (p) 
        free(p);
    else
        this->events.lower.emit(this);
};


rfir::util::Mem GMem;

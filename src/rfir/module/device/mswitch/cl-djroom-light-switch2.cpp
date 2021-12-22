#include "cl-djroom-light-switch2.h"


void rfir::module::device::mswitch::CL_DJROOM_Light_Switch2::start(void * p) {
    MSwitch::start(p);
    auto sw1 = new Switch();
    sw1->params.name = "state_l1";
    sw1->params.enabled = true;
    sw1->params.pinIn = 12;
    sw1->params.pinOut = 14;

    auto sw2 = new Switch();
    sw2->params.name = "state_l2";
    sw2->params.enabled = true;
    sw2->params.pinIn = 13;
    sw2->params.pinOut = 16;    
    
    addSwitch(sw1);
    addSwitch(sw2);

};

void rfir::module::device::mswitch::CL_DJROOM_Light_Switch2::loop() {
    MSwitch::loop();    

}; 

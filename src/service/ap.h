#ifndef SERVICE_AP_H
#define SERVICE_AP_H

#include "rfir/util/button.h"

namespace service {
    class AP {     
    public:
        rfir::util::Button::KeyTime keyTimeResetConfig = {pressed: 10000, released: 15000};
    public:
        void init();
        void uninit();
        void start();
        void stop();
        void loop();
    public:
        void resetConfig();
        void* onAPConfigSaved(void* arg, void* p);
        void* onAPApplyDefault(void* arg, void* p);
    };

}

extern service::AP GAP;     

#endif                

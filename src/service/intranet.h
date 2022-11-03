#ifndef SERVICE_INTRANET_H
#define SERVICE_INTRANET_H


#include "WiFiUdp.h"

namespace service {
    class Intranet {  
    public:
        struct Params {
            char*  buf = 0;
            size_t bufsize = 0;
        };

    public:
        Params params;
        WiFiUDP* udp = 0;

    public:
        void init();
        void uninit();
        void start(bool init);
        void start();
        void stop();
        void loop();
    public:
        void read();
        int write(char* buf, size_t size);
        void req_intranet();
        void dump(char* buf, size_t size);
    };

}

extern service::Intranet GIntranet;     

#endif                

#ifndef SERVICE_CMDS_H
#define SERVICE_CMDS_H

#include "cmds/cmd/cmd.h"

namespace service {
    class Cmds {     
    public:
        void start();
        void loop();
    public:
        void onCmd(cmds::cmd::RecvCmd* cmd);
    };

}

extern service::Cmds GCmds;     

#endif                

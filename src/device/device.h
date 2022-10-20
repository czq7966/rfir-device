#ifndef DEVICE_DEVICE_H
#define DEVICE_DEVICE_H

#include "cmds/cmd/cmd.h"

namespace device {
    class Device {

    public:
        virtual void start();
        virtual void loop();
    public:
        virtual int onCmd_config(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_handshake(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_reboot(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_get(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_set(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_report(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_penet(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_update(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_reset_config(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_device_joined(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_device_leave(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_device_interview(cmds::cmd::RecvCmd* cmd);
    };
    
}
                

#endif                
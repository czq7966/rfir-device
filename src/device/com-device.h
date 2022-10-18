#ifndef DEVICE_COM_EVICE_H
#define DEVICE_COM_DEVICE_H

#include "cmds/cmd/cmd.h"
#include "device.h"

namespace device {
    class ComDevice: public Device {
    public:
        HardwareSerial* serial;    

    public:
        virtual void start() override;
        virtual void loop() override;
        virtual size_t read();
        virtual size_t write(const char* buf, const size_t size);
    public:
        virtual int onCmd_config(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_handshake(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_reboot(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_get(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_set(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_report(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_penet(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_update(cmds::cmd::RecvCmd* cmd) override;    

    };
    
}
                

#endif                
#ifndef DEVICE_COM_DEVICE_H
#define DEVICE_COM_DEVICE_H

#include "cmds/cmd/cmd.h"
#include "rfir-device.h"

namespace device {
    class ComDevice: public RfirDevice {
    public:
        HardwareSerial* serial;    
    public:
        virtual void start() override;
        virtual void loop() override;
        virtual size_t read();
        virtual size_t write(const char* buf, const size_t size);
        virtual size_t write(std::vector<uint8_t>* buf, const size_t size, const size_t offset);
    public:
        virtual int onCmd_config(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_handshake(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_reboot(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_get(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_set(cmds::cmd::RecvCmd* cmd);
        virtual int onCmd_report(cmds::cmd::RecvCmd* cmd) override;
        virtual int onCmd_penet(cmds::cmd::RecvCmd* cmd, int offset = 0) override;
        virtual int onCmd_update(cmds::cmd::RecvCmd* cmd) override;    

    };
    
}
                

#endif                
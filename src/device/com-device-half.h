#ifndef DEVICE_COM_DEVICE_HALF_H
#define DEVICE_COM_DEVICE_HALF_H

#include "cmds/cmd/cmd.h"
#include "com-device.h"

namespace device {
    class ComDeviceHalf: public ComDevice {
    public:
        virtual void readMode();
        virtual void writeMode();
    public:
        virtual void start() override;
        virtual size_t write(const char* buf, const size_t size);
    };
    
}
                

#endif                
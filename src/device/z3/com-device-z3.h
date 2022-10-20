#ifndef DEVICE_COM_DEVICE_Z3_H
#define DEVICE_COM_DEVICE_Z3_H

#include "cmds/cmd/cmd.h"
#include "../com-device.h"

namespace device {
    class ComDeviceZ3: public ComDevice {

    public:
        virtual void start() override;
    };
    
}
                

#endif                
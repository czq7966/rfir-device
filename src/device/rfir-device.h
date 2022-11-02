#ifndef DEVICE_RFIR_DEVICE__H
#define DEVICE_RFIR_DEVICE__H

#include "cmds/cmd/cmd.h"
#include "device.h"
#include "rfir/ttl/sniffer.h"

namespace device {
    class RfirDevice: public Device {
    public:
        virtual void start() override;
    public:
        virtual int on_rfir_sniffed(rfir::ttl::Sniffer::Data* data);
    };
    
}
                

#endif                
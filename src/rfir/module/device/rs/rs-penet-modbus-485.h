#ifndef __RFIR_MODULE_DEVICE_RS_RS_PENET_MODBUS_485_H__
#define __RFIR_MODULE_DEVICE_RS_RS_PENET_MODBUS_485_H__


#include "rs-penet-485.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace RS {
                class RSPenetModbus485: public RS::RSPenet485 {
                public:
                    virtual void init() override;         
                    virtual bool checkSumCode(uint8_t* buf, uint8_t len) override;        
                    virtual bool onSvc_penet(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;                    
                };
            }
        }
    }
}



#endif //

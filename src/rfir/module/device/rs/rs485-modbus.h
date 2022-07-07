#ifndef __RFIR_MODULE_DEVICE_RS_RS485_MODBUS_H__
#define __RFIR_MODULE_DEVICE_RS_RS485_MODBUS_H__


#include "rs485.h"
#include "../../../util/map-factory.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace RS {
                class RS485_MODBUS: public RS::RS485 {
                public:
                    uint8_t m_modAddr = 1;
                    rfir::util::MapFactory<std::string, uint16_t> m_regAddrs;
                public:
                    virtual bool checkSumCode(uint8_t* rx_buf, uint8_t len) override; 
                public:
                    virtual void init() override;
                    virtual void loop() override;
                public:
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;                     
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;     
                public:
                    virtual bool splitRegAddr(uint32_t regAddr, int8_t& func, uint16_t& addr, int writeOffset = -1);
                    virtual bool getRegOne(uint8_t modAddr, uint32_t regAddr, uint32_t& value, uint32_t& error);
                    virtual bool setRegOne(uint8_t modAddr, uint32_t regAddr, uint32_t value, uint32_t& error);
                };
            }
        }
    }
}



#endif //

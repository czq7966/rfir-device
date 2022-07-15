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
                    class Cmd {
                    public:
                        uint8_t buffer[256];

                        uint8_t  id;
                        uint8_t  func;
                        uint16_t addr;
                        uint16_t len;
                        uint16_t val;
                        uint16_t sum;
                        virtual void fixUp();
                        virtual bool getRaw(uint8_t& buf, uint8_t& size);
                    };

                    class Cmd_Read_Coils: public Cmd {
                    public:
                        Cmd_Read_Coils();
                    public:
                        virtual void fixUp() override;
                        virtual bool getRaw(uint8_t& buf, uint8_t& size) override;
                    };

                    class Cmd_Read_Discrete: public Cmd_Read_Coils {
                    public:
                        uint8_t  func = 0x02;
                    };        

                    union Cmd_Read_Holding
                    {
                        uint8_t buffer[8];
                        struct {
                            uint8_t  id;
                            uint8_t  func = 0x03;
                            uint16_t addr;
                            uint16_t len;
                            uint16_t sum;
                        };
                    };                       
                    union Cmd_Read_Input
                    {
                        uint8_t buffer[8];
                        struct {
                            uint8_t  id;
                            uint8_t  func = 0x04;
                            uint16_t addr;
                            uint16_t len;
                            uint16_t sum;
                        };
                    };  

                    union Cmd_Write_Single_Coil
                    {
                        uint8_t buffer[8];
                        struct {
                            uint8_t  id;
                            uint8_t  func = 0x04;
                            uint16_t addr;
                            uint16_t len;
                            uint16_t sum;
                        };
                    };                      
                    
                public:
                    uint8_t m_modAddr = 1;
                    rfir::util::MapFactory<std::string, uint16_t> m_regAddrs;
                public:
                    virtual bool checkSumCode(uint8_t* rx_buf, uint8_t len) override; 
                public:
                    virtual void init() override;
                    virtual void loop() override;
                public:
                    virtual int onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;                     
                    virtual int onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;     
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

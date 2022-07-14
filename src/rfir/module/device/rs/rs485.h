#ifndef __RFIR_MODULE_DEVICE_RS_RS485_H__
#define __RFIR_MODULE_DEVICE_RS_RS485_H__

#include "../device.h"
#include "rs485.h"

#ifndef RS485_MAX_BUF_LEN
#define RS485_MAX_BUF_LEN 256
#endif

namespace rfir {
    namespace module {
        namespace device {
            namespace RS {
                class RS485: public Device {
                public:
                public:
                    HardwareSerial* hwSerial;
                    uint8_t dePin = 5;
                    uint8_t rePin = 5;
                    uint8_t rxBuf[RS485_MAX_BUF_LEN];
                    uint8_t txBuf[RS485_MAX_BUF_LEN];
                    uint32_t recvTimeoutMS = 100;
                public:
                    void readMode();
                    void writeMode();                
                public:
                    virtual bool sendCode(uint8_t* tx_buf, uint8_t len);
                    virtual bool recvCode(uint8_t* rx_buf, uint8_t& len, unsigned long timeout_ms);
                    virtual bool sendCodeAndRecv(uint8_t* tx_buf, uint8_t tx_len, uint8_t* rx_buf, uint8_t& rx_len, unsigned long timeout_ms);
                    virtual bool checkSumCode(uint8_t* buf, uint8_t len);

                public:
                    virtual void init() override;
                    virtual void loop() override;
                    virtual int onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;                     
                    virtual int onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 

                    virtual bool onSvc_setBandRate(neb::CJsonObject* pld); 
                };
            }
        }
    }
}
                

#endif                

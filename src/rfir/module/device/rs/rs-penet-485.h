#ifndef __RFIR_MODULE_DEVICE_RS_RS_PENET_485_H__
#define __RFIR_MODULE_DEVICE_RS_RS_PENET_485_H__


#include "rs-penet.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace RS {
                class RSPenet485: public RS::RSPenet {
                public:
                    uint8_t dePin = 5;
                    uint8_t rePin = 5;     
                    int recvTimeout = 100;                           
                public:
                    virtual void readMode();
                    virtual void writeMode();                
                public:
                    virtual bool sendCode(uint8_t* tx_buf, int len);
                    virtual bool recvCode(uint8_t* rx_buf, int& len, unsigned long timeout_ms);
                    virtual bool sendCodeAndRecv(uint8_t* tx_buf, int tx_len, uint8_t* rx_buf, int& rx_len, unsigned long timeout_ms);
                    virtual bool checkSumCode(uint8_t* buf, uint8_t len);     
                public:
                    virtual int  rsRead(char buffer[], int offset = 0);
                    virtual bool rsWrite(char buffer[], size_t length, int offset = 0);                                        
                public:
                    virtual void init() override;                  
                    virtual void initDerePin(); 
                };
            }
        }
    }
}



#endif //

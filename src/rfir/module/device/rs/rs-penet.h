#ifndef __RFIR_MODULE_DEVICE_RS_RS_PENET_H__
#define __RFIR_MODULE_DEVICE_RS_RS_PENET_H__


#include "../device.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace RS {
                class RSPenet: public Device {
                public:
                    static const uint16_t RsBufferMax = 2 * 1024;
                public:
                    HardwareSerial* hwSerial;
                public:
                    char rsBuffer[RsBufferMax];
                    virtual int  rsRead(char buffer[], int offset = 0);
                    virtual bool rsWrite(char buffer[], size_t length, int offset = 0);
                    virtual std::string rsReadBase64(char data[]);
                    virtual bool rsWriteBase64(char data[], size_t size);

                public:
                    virtual void loop() override;   
                public:
                    virtual void init() override;            
                    virtual void initSerial();   
                public:
                    virtual bool doEvt_penet();
                    virtual bool onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                    virtual bool onSvc_penet(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) override;  
                };
            }
        }
    }
}



#endif //

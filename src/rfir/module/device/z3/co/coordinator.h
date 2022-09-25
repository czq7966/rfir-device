#ifndef __RFIR_MODULE_DEVICE_Z3_CO_COORDINATOR_H__
#define __RFIR_MODULE_DEVICE_Z3_CO_COORDINATOR_H__


#include "../../device.h"

#ifndef COSerial
#define COSerial Serial
#endif


namespace rfir {
    namespace module {
        namespace device {
            namespace z3 {
                namespace co {
                    class Coordinator: public Device {
                    public:
                        static const uint16_t CoBufferMax = 3 * 1024;

                    public:                     
                        Coordinator();
                        ~Coordinator();
                    public:
                        char coBuffer[CoBufferMax];
                        int  coRead(char buffer[]);
                        bool coWrite(char buffer[], size_t length);
                        std::string coReadBase64(char data[]);
                        bool coWriteBase64(char data[], size_t size);
                    public:
                        virtual void start(void *) override;
                        virtual void loop() override;   
                    public:
                        virtual void init() override;            
                    public:
                        virtual bool doEvt_penet();
                        virtual int onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                        virtual int onSvc_set(JsonObject* pld, cmds::cmd::CmdBase* cmd) override; 
                        virtual int onSvc_penet(JsonObject* pld, cmds::cmd::CmdBase* cmd) override;                    
                    };
                }
            }
        }
    }
}



#endif //

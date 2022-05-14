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
                        virtual void doTimerReport(bool reset = false) override;                    
                    public:
                        virtual rfir::module::ttl::Config::Device* init() override;            
                        virtual bool setRaw(uint8_t* raw) override;
                        virtual uint8_t* getRaw(int& count) override;       
                        virtual uint16_t* getEncodeRaw(int& count);         
                        virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                        virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                        virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override;                         
                        void dump();
                    public:
                        virtual bool doEvt_penet();
                        virtual bool onSvc_get(neb::CJsonObject* pld) override; 
                        virtual bool onSvc_set(neb::CJsonObject* pld) override; 
                        virtual bool onSvc_penet(neb::CJsonObject* pld) override;                    
                    };
                }
            }
        }
    }
}



#endif //

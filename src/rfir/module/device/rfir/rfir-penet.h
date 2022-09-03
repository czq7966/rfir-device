#ifndef __RFIR_MODULE_DEVICE_RFIR_RFIR_PENET_H__
#define __RFIR_MODULE_DEVICE_RFIR_RFIR_PENET_H__


#include "../rfir-device.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace rfir{
                class RFIRPenet: public RFIRDevice {
                private:
                    ::rfir::module::ttl::SnifferV2::Data* m_sniffed_data = 0;

                public:
                    virtual void init() override;   
                    virtual bool loadConfig() override;  
                    virtual bool saveConfig() override;  
                    virtual bool setConfig(neb::CJsonObject* config);
                public:
                    virtual void* onSniffed(void* arg, void* p);
                    virtual void* onSended(void* arg, void* p);
                public:
                    virtual bool rfirWrite(char buffer[], size_t length, int offset = 0);
                    virtual bool rfirWrite(uint16_t buffer[], size_t length, int offset = 0);
                    virtual bool rfirWriteBase64(char data[], size_t size);
                public:
                    virtual int onSvc_get(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_set(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) override; 
                    virtual int onSvc_config(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) override;
                    virtual int onSvc_penet(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) override;  
                };
            }
        }
    }
}



#endif //

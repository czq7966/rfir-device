#ifndef __RFIR_MODULE_DEVICE_RFIR_DEVICE_H__
#define __RFIR_MODULE_DEVICE_RFIR_DEVICE_H__


#include "device.h"
#include "rfir/module/ttl/sniffer-v2.h"
#include "rfir/module/ttl/decoder-v2.h"
#include "rfir/module/ttl/encoder-v2.h"
#include "rfir/module/ttl/sender-v2.h"



namespace rfir {
    namespace module {
        namespace device {
            class RFIRDevice: public Device {
            public:
                ::rfir::module::ttl::SnifferV2* sniffer = 0;
                ::rfir::module::ttl::DecoderV2* decoder = 0;
                ::rfir::module::ttl::EncoderV2* encoder = 0;
                ::rfir::module::ttl::SenderV2*  sender  = 0;
            public:
                virtual void* onSniffed(void* arg, void* p);
                virtual void* onDecoded(void* arg, void* p);
                virtual void* onEncoded(void* arg, void* p);
                virtual void* onSended(void* arg, void* p);

            public:
                RFIRDevice();
                ~RFIRDevice();
            public:
                std::string getRaw();                 
                bool setRaw(const char* raw); 
            public:
                virtual bool setConfig(const char* context) override;
                virtual bool getConfig(std::string& context) override; 


                // virtual bool loadRaw();      
                // virtual bool saveRaw();    
                // bool setRaw(const char* rawStr);         
                virtual bool getEncodeRaw(std::list<uint16_t>& result);                 
                                           
            public:
                virtual void start(void *) override;
                virtual void loop() override;   
                virtual uint8_t* getRaw(int& count); 
                virtual bool setRaw(uint8_t* raw);
                virtual bool sendRaw(); 

                virtual int onSvc_get(JsonObject* pld, ::cmds::cmd::CmdBase* cmd) override; 
                virtual int onSvc_set(JsonObject* pld, ::cmds::cmd::CmdBase* cmd) override;
                virtual int onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& p);   

            };
        }

    }
}



#endif //

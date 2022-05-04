#ifndef __RFIR_MODULE_DEVICE_DEVICE_H__
#define __RFIR_MODULE_DEVICE_DEVICE_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/rfir.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/gpio.h"
#include "rfir/module/ttl/config.h"
#include "cmds/cmd/cmd-base.h"

namespace rfir {
    namespace module {
        namespace device {
            class Device {
            public:
                struct Events
                {
                    rfir::util::Event onChange;
                };
                
                typedef std::function<void(void* device, const char* reason)> OnChange;
                std::string name;
                Events events;
                void emitChange(const char* reason);
            public:
                int timerReport_LastTime = 0;
                int timerReport_Interval = 60 * 1000;      
                bool rawChanged = false;    
                unsigned long  rawChanged_time = 0; 
                unsigned long  rawChanged_timeout = 2000; 
            public:
                virtual void start(void *);
                virtual void loop();
                virtual void onSetRaw();
                virtual bool loadRaw();      
                virtual bool saveRaw();      
                virtual void doTimerReport(bool reset = false);    
                virtual void reinitTimerReport(bool reset = false);
                virtual void doRawChanged();    
                virtual std::string toBitString();
                virtual std::string toHexString();

            public: // subclass need implement below
                virtual rfir::module::ttl::Config::Device* init();            
                virtual bool setRaw(uint8_t* raw);
                virtual uint8_t* getRaw(int& count);                
                virtual uint16_t* getEncodeRaw(int& count);
                virtual bool getCommonProps(neb::CJsonObject* pld);
                virtual bool getProps(neb::CJsonObject* pld);
                virtual bool onCmd_set(neb::CJsonObject* pld); 
                virtual bool onCmd_get(neb::CJsonObject* pld); 
                virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) ; 
            };

            class Networking {
            public:
                bool m_logined = false;
                int  m_handshake_failed_count = 0;
            public:
                ~Networking();
                virtual void start();
                virtual void loop();
            public:
                bool login();
                bool handshake();
                bool heartbeat();
                void setWill();
                void setOnline();
            public:
                // void onLoginReq(::cmds::cmd::CmdBase* cmd);
                // void onLoginResp(::cmds::cmd::CmdBase* cmd, void* _this = 0);
                // void onLoginTimeout(uint32_t sid);

                // void onHandshakeReq(::cmds::cmd::CmdBase* cmd);
                // void onHandshakeResp(::cmds::cmd::CmdBase* cmd);
                // void onHandshakeTimeout(uint32_t sid);
            public:
                void* onConnect(void* arg, void* p);
                void* onCommand(void* arg, void* p);
            public:
                void* doLogin(void* arg, void* p);
                void* onLoginReq(void* arg, void* p);
                void* onLoginResp(void* arg, void* p);
                void* onLoginTimeout(void* arg, void* p);
                void* onHandshakeReq(void* arg, void* p);
                void* onHandshakeResp(void* arg, void* p);                
                void* onHandshakeTimeout(void* arg, void* p);
            };

        }
    }
}

extern rfir::module::device::Device* GDevice;

#endif //

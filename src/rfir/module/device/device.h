#ifndef __RFIR_MODULE_DEVICE_DEVICE_H__
#define __RFIR_MODULE_DEVICE_DEVICE_H__


#include <Arduino.h>
#include "config.h"
#include "cmds/cmd/cmd-base.h"

namespace rfir {
    namespace module {
        namespace device {
            class Device {
            public:
                struct Events
                {
                    rfir::util::Event onChange;
                    rfir::util::Event onEvtPenet;
                    rfir::util::Event onEvtPropsChange;
                };
                
                std::string name;
                Events events;

            public:
                virtual void init(); 
                virtual void start(void *);
                virtual void loop();

                virtual std::string getConfigFilename();
                virtual bool loadConfig();      
                virtual bool saveConfig();  
                virtual void delaySaveConfig(int timeout_ms = 1000); 
                virtual void* doSaveConfig(void* , void*);  
                virtual bool setConfig(const char* context);
                virtual bool getConfig(std::string& context);   

            public: // subclass need implement below
                          
            //V2
            public:
                uint32_t m_timer_report_handler = 0;
            public:
                virtual bool getCommonProps(JsonObject* pld);
                virtual bool getProps(JsonObject* pld, ::cmds::cmd::CmdBase* cmd);
            public:
                virtual bool doEvtTimerReport(uint32_t timeout = DEVICE_TIMER_REPORT_TIMEOUT);
                virtual void* onEvt_timer_report(void* arg, void* p);
            public:
                virtual void* doSvc_reboot(void* arg, void* p);
            public:
                virtual int onSvc_get(JsonObject* pld, ::cmds::cmd::CmdBase* cmd); 
                virtual int onSvc_set(JsonObject* pld, ::cmds::cmd::CmdBase* cmd); 
                virtual int onSvc_config(JsonObject* pld, ::cmds::cmd::CmdBase* cmd);
                virtual int onSvc_reboot(JsonObject* pld, ::cmds::cmd::CmdBase* cmd);
                virtual int onSvc_penet(JsonObject* pld, ::cmds::cmd::CmdBase* cmd);
            };

        }
    }
}

extern rfir::module::device::Device* GDevice;

// extern rfir::module::device::Networking GNetworking;

#endif //

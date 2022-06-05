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
                    rfir::util::Event onEvtPenet;
                    rfir::util::Event onEvtPropsChange;
                };
                
                // typedef std::function<void(void* device, const char* reason)> OnChange;
                std::string name;
                Events events;
            //     void emitChange(const char* reason);
            // public:
            //     int timerReport_LastTime = 0;
            //     int timerReport_Interval = 60 * 1000;      
            //     bool rawChanged = false;    
            //     unsigned long  rawChanged_time = 0; 
            //     unsigned long  rawChanged_timeout = 2000; 
            public:
                virtual void init(); 
                virtual void start(void *);
                virtual void loop();

                virtual bool loadConfig();      
                virtual bool saveConfig();  
                virtual void delaySaveConfig(int timeout_ms = 1000); 
                virtual void* doSaveConfig(void* , void*);  
                virtual bool setConfig(const char* context);
                virtual bool getConfig(std::string& context);   

                // virtual void onSetRaw();
                // virtual void doTimerReport(bool reset = false);    
                // virtual void reinitTimerReport(bool reset = false);
                // virtual void doRawChanged();    
                // virtual std::string toBitString();
                // virtual std::string toHexString();

            public: // subclass need implement below
                          
      

                // virtual bool onCmd_set(neb::CJsonObject* pld); 
                // virtual bool onCmd_get(neb::CJsonObject* pld); 
                // virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) ; 
            //V2
            public:
                uint32_t m_timer_report_handler = 0;
            public:
                virtual bool getCommonProps(neb::CJsonObject* pld);
                virtual bool getProps(neb::CJsonObject* pld);
            public:
                virtual bool doEvtTimerReport(uint32_t timeout = DEVICE_TIMER_REPORT_TIMEOUT);
                virtual void* onEvt_timer_report(void* arg, void* p);
            public:
                virtual void* doSvc_reboot(void* arg, void* p);
            public:
                virtual bool onSvc_get(neb::CJsonObject* pld); 
                virtual bool onSvc_set(neb::CJsonObject* pld); 
                virtual bool onSvc_reboot(neb::CJsonObject* pld);
                virtual bool onSvc_penet(neb::CJsonObject* pld);
            };

        }
    }
}

extern rfir::module::device::Device* GDevice;

// extern rfir::module::device::Networking GNetworking;

#endif //
